#include <Servo.h>
#include "HUSKYLENS.h"
#include <SoftwareSerial.h>
#include <DHT.h>
Servo myServo;

//main sensors
const int soundpin = 2; //add sound bits
const int motionsensor = 13; //
const int noisesensor = 10;//
const int temp_humiditysensor = 20; //



// other functions
const int redpin = 0;
const int greenpin = 0;
const int bluepin = 0;
const int buttonswitch = 3;
boolean locked = false;

#define DHTTYPE DHT11
DHT dht(temp_humiditysensor, DHTTYPE);

HUSKYLENS huskylens;
bool systemArmed = false;
bool lastButtonState = LOW;

const float Regular_Room_Temp = 77.0;

//int knockVal;
//int statusofbutton = 0;

void setup() {
  pinMode(soundpin, OUTPUT);
  pinMode(motionsensor, INPUT);
  pinMode(noisesensor, INPUT);
  pinMode(buttonswitch, INPUT_PULLUP);
  
  dht.begin();
  Wire.begin();
  myServo.attach(9);
  myServo.write(0);
  

  while (!huskylens.begin(Wire)) {
    Serial.println(F("Your virtual dog is hungry foo"));
    delay(1000);
  }
  Serial.println(F("Your virtual pet is starting up ..."));
  
  setRGBColor(0, 0, 255); // Blue means unarmed
}

void loop() {
  // 1. Check On/Off Button Switch
  bool ButtonStatus = digitalRead(buttonswitch);
  if (ButtonStatus == LOW && lastButtonState == HIGH) {
    systemArmed = !systemArmed;
    delay(200); // Debounce delay
    if (systemArmed && locked == false) {
      locked = true;
      myServo.write(90);
      Serial.println(F("Arms up, lock active"));
      setRGBColor(255, 0, 0); // Red for Armed
    } else {
      locked = false;
      myServo.write(0);
      Serial.println(F("Arms down, lock loose"));
      setRGBColor(0, 0, 255); // Blue for Disarmed
    }
  }

  lastButtonState = ButtonStatus;

  // Security Checks (Only if Armed)
  if (systemArmed) {
    // checking humidity and temperature
    float humidity = dht.readHumidity();
    float tempF = dht.readTemperature(true);

    // Read sound and motion sensors   // change the name so that its noiseSensor, easier to read
    int soundDetected = digitalRead(noisesensor);
    int motionDetected = digitalRead(motionsensor);
    
    if (soundDetected == HIGH) {
      Serial.println(F("Sound detected! Checking motion..."));
      
      if (motionDetected == HIGH) {
        Serial.println(F("Yoo motion detected after sound! Activating HuskyLens..."));
        triggerAlarm(); // Optional visual cue
        activateHuskyLensScan();
      }
    } 
    else {
      // If sound and motion did NOT activate, check temperature fallback
      if (!isnan(tempF) && tempF > Regular_Room_Temp) {
        Serial.print(F("Temp is high ("));
        Serial.print(tempF);
        Serial.println(F("F)! Activating HuskyLens fallback scan..."));
        activateHuskyLensScan();
      }
    }
  }

  delay(200); // Small delay between loops
}


void setRGBColor(int red, int green, int blue) {
  analogWrite(redpin, red);
  analogWrite(greenpin, green);
  analogWrite(bluepin, blue);
}

void triggerAlarm() {
  // Flash RGB LED purple/white
  for (int i = 0; i < 3; i++) {
    setRGBColor(255, 0, 255); // Purple flash
    delay(200);
    setRGBColor(255, 255, 255);
    delay(200);
  }
  setRGBColor(255, 0, 0); // Return to armed red
}

void activateHuskyLensScan() {
  // need to register faces to not shot on sight
  if (huskylens.request()) {
    if (huskylens.isLearned()) {
      Serial.println(F("AI Alert: Face/Object recognized and registered!"));
    } else {
      Serial.println(F("AI Alert: Moving object detected, but face is not learned/recognized yet."));
    }
  } else {
    Serial.println(F("Check your wires, your dog might have bitten"));
  }
}


//need to make rgb better, make the sound integrated into the system