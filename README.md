# EGEN-310R
## User
#### APP

![interface](./emulator.jpg)

RC_TANK.apk can be downloaded onto an android phone. To initialoze BLE configuration, press Scan and select the BT device to connect to, an Adafruit Bluefruit with the 310BLE_car compiled onto it is what will communicate with this app. The controls simply follw the control pad -- forward, left, right, and backward.

#### Bluefruit

The folder /310BLE_car can be compiled from the arduino IDE and uploaded to your device.

## DEV
#### APP

This app was developed in MITappBuilder2. The .apk can be uploaded and developed from that website. Due to no longer having an android phone to test on(thanks COVID), this version of the app was only tested on their emulator. No functional differences have changed since prototype 1 but several aesthetic changes have been made so I believe the APP should still work to communicate with the Bluefruit.

#### Bluefruit

These files can be edited in any editor, they have been mostly adapted from https://github.com/adafruit/Adafruit_Learning_System_Guides/tree/master/BLE_Robot_Rover. The difference from prototype 1 to prototype 2 were that I added 2 extra motors to the functions, cleaned up excess code, and added comments. Again, without access to the bluefruit I assume that the code still works with the app. The code does compile so I can only assume it still works.
