/* This code has been adapted from 
 * https://github.com/adafruit/Adafruit_Learning_System_Guides/tree/master/BLE_Robot_Rover 
 * by Jimmy Boland in order to control Group C.1's RC Tank and communicate with the RC_TANK APP 
 */


#include <Arduino.h>
#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>

#include "BluefruitConfig.h"

#include <Adafruit_MotorShield.h>

#define ForwardSpeed                255
#define ReverseSpeed                255
#define TurningSpeed                100


// creating global motor variables 

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *L_MOTOR1 = AFMS.getMotor(1);
Adafruit_DCMotor *R_MOTOR1 = AFMS.getMotor(2);
Adafruit_DCMotor *L_MOTOR2 = AFMS.getMotor(3);
Adafruit_DCMotor *R_MOTOR2 = AFMS.getMotor(4);

String BROADCAST_NAME = "310 CAR";

String BROADCAST_CMD = String("AT+GAPDEVNAME=" + BROADCAST_NAME);

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// simple error catch

void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

// call to parse packet

uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);

extern uint8_t packetbuffer[];

char buf[60];

void setup() {

  //initialize motors and BLE; set start speed to 0
  Serial.begin(9600);

  AFMS.begin();

  L_MOTOR1->setSpeed(0);
  L_MOTOR1->run(RELEASE);

  L_MOTOR2->setSpeed(0);
  L_MOTOR2->run(RELEASE);

  R_MOTOR1->setSpeed(0);
  R_MOTOR1->run(RELEASE);

  R_MOTOR2->setSpeed(0);
  R_MOTOR2->run(RELEASE);
    
  Serial.begin(115200);

  BLEsetup();

}

void loop() {

  // continuousl read read packet being sent from app
  uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);

  readController();
}

bool isMoving = false;
unsigned long lastPress = 0;

bool readController(){
  uint8_t maxspeed;

  if (packetbuffer[1] == 'B') {

    uint8_t buttnum = packetbuffer[2] - '0';
    boolean pressed = packetbuffer[3] - '0';

    if (pressed) {

      if(buttnum == 5){
        // forward is pressed
        
        isMoving = true;
        L_MOTOR1->run(FORWARD);
        L_MOTOR2->run(FORWARD);
        R_MOTOR1->run(FORWARD);
        R_MOTOR2->run(FORWARD);
        maxspeed = ForwardSpeed;
      }
      
      if(buttnum == 6){
        // backward is pressed
        
        isMoving = true;
        L_MOTOR1->run(BACKWARD);
        L_MOTOR2->run(BACKWARD);
        R_MOTOR1->run(BACKWARD);
        R_MOTOR2->run(BACKWARD);
        maxspeed = ReverseSpeed;        
      }
      
      if(buttnum == 7){

        // left is pressed
        
        isMoving = true;
        L_MOTOR1->run(BACKWARD);
        L_MOTOR2->run(BACKWARD);
        R_MOTOR1->run(FORWARD);
        R_MOTOR2->run(FORWARD);
        maxspeed = TurningSpeed;
      }
      
      if(buttnum == 8){

        // right is pressed 
        
        isMoving = true;
        L_MOTOR1->run(FORWARD);
        L_MOTOR2->run(FORWARD);
        R_MOTOR1->run(BACKWARD);
        R_MOTOR2->run(BACKWARD);
        maxspeed = TurningSpeed;        
      }

      lastPress = millis();

      // speed up the motors gradually (no wheelies etc)
      
      for (int speed=0; speed < maxspeed; speed+=5) {
        L_MOTOR1->setSpeed(speed);
        L_MOTOR2->setSpeed(speed);
        R_MOTOR1->setSpeed(speed);
        R_MOTOR2->setSpeed(speed);
        delay(5); // 250ms total to speed up
      }
    }
    else {
        isMoving = false;
        // slow down the motors (no forward rolls)
        for (int speed = maxspeed; speed >= 0; speed-=5) {
          L_MOTOR1->setSpeed(speed);
          L_MOTOR2->setSpeed(speed);
          R_MOTOR1->setSpeed(speed);
          R_MOTOR2->setSpeed(speed);
          delay(5); // 50ms total to slow down
        }
        L_MOTOR1->run(RELEASE);
        L_MOTOR2->run(RELEASE);
        R_MOTOR1->run(RELEASE);
        R_MOTOR2->run(RELEASE);
    }
  }
}

void BLEsetup() {

    // initialize BLE communication
    
    if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );
  
  Serial.println(F("Performing a factory reset: "));
  if (! ble.factoryReset() ){
       error(F("Couldn't factory reset"));
  }
  
  BROADCAST_CMD.toCharArray(buf, 60);

  if(ble.sendCommandCheckOK(buf)){
    Serial.println("name changed");
  }
  delay(250);

  if(ble.sendCommandCheckOK("ATZ")){
    Serial.println("resetting");
  }
  delay(250);

  ble.sendCommandCheckOK("AT+GAPDEVNAME");

  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println();

  ble.verbose(false);

  while (! ble.isConnected()) {
      delay(500);
  }
  Serial.println(F("*****************"));

  // Set Bluefruit to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("*****************"));  
}
