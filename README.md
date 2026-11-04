# ENS112

## Home Security System

This Arduino project is a prototype home security system that combines motion and sound sensing with a HuskyLens camera module. A button toggles the system between armed and disarmed states, and a servo moves to indicate or operate the lock. When armed, the program checks for sound followed by motion and can flash an RGB indicator before scanning with HuskyLens. It also starts a HuskyLens scan when the DHT11 temperature reading rises above 77°F. Status and recognition messages are printed over Serial.

The sketch uses a DHT11 temperature and humidity sensor, a motion sensor, a sound sensor, a HuskyLens module, a servo, an RGB LED, and a push button. Face or object recognition depends on what has been learned by the HuskyLens.

Phoenix Allen

