<<<<<<< Updated upstream
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
  

=======
#include "HUSKYLENS.h"
#include <SoftwareSerial.h>
#include <DHT.h>


//main sensors
const int soundpin = 2; //
const int motionsensor = 13; //
const int noisesensor = 10;//
const int tempandhumiditysensor = 20; //
const int knocklock = 6;
const int huskylens = 9;

// other functions
const int redpin = 9;
const int greenpin = 10;
const int bluepin = 11;
const int buttonswitch = 9;

#define DHTTYPE DHT11
DHT dht(dhtPin, DHTTYPE);

HUSKYLENS huskylens;
bool systemArmed = false;
bool lastButtonState = HIGH;


int knockVal;
int statusofbutton = 0;

void setup() {
  //no water leak sensor pin
  pinMode(motionsensor, INPUT);
  pinMode(noisesensor, INPUT);
  pinMode(knocklock, OUTPUT);
  pinMode(buttonswitch,INPUT_PULLUP);
  dht.begin();

  Wire.begin();
>>>>>>> Stashed changes
  while (!huskylens.begin(Wire)) {
    Serial.println(F("Your virtual dog is hungry foo"));
    delay(1000);
  }
  Serial.println(F("Your virtual pet is starting up ..."));
  
<<<<<<< Updated upstream
  setRGBColor(0, 0, 255); // Blue means unarmed
=======
  setRGBColor(0, 0, 255); // Blue means Standby/Disarmed
>>>>>>> Stashed changes
}

void loop() {
  // 1. Check On/Off Button Switch
<<<<<<< Updated upstream
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
=======
  bool currentButtonState = digitalRead(buttonswitch);
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    systemArmed = !systemArmed;
    delay(200); // Debounce delay
    if (systemArmed) {
      Serial.println(F("Arms"));
      setRGBColor(255, 0, 0); // Red for Armed
    } else {
      Serial.println(F("System DISARMED."));
      setRGBColor(0, 0, 255); // Blue for Disarmed
    }
  }
  lastButtonState = currentButtonState;

  //waterleak sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  //int waterLeak = digitalRead(waterleaksensorpin);
  
  //if (waterLeak == HIGH) {
  //  Serial.println(F("Yoo the water is leakingg"));
  //  triggerAlarm();
  //}

  // 3. Security Checks (Only if Armed)
  if (systemArmed) {
    // Check Motion
    int motionDetected = digitalRead(motionsensor);
    if (motionDetected == HIGH) {
      Serial.println(F("Yoo someone moving up in your joint!"));
      triggerAlarm();
    }

    // Check Noise
    int noiseDetected = digitalRead(noisesensor);
    if (noiseDetected == HIGH) {
      Serial.println(F("Yoo someone is taking your food dawg"));
      triggerAlarm();
    }

    // Check AI Camera (HuskyLens)
    if (huskylens.request()) {
      if (huskylens.isLearned()) {
        Serial.println(F("AI Alert: SOMEONES TAKING YOUR DOG MAN"));
        // Add custom logic here (e.g., unlock mechanism if recognized)
>>>>>>> Stashed changes
      }
    }
  }

<<<<<<< Updated upstream
  delay(200); // Small delay between loops
}

=======
  delay(100);
}

// --- Helper Functions ---
>>>>>>> Stashed changes

void setRGBColor(int red, int green, int blue) {
  analogWrite(redpin, red);
  analogWrite(greenpin, green);
  analogWrite(bluepin, blue);
}

void triggerAlarm() {
<<<<<<< Updated upstream
  // Flash RGB LED purple/white
=======
  // Flash RGB LED purple/white or sound buzzer/lock
>>>>>>> Stashed changes
  for (int i = 0; i < 3; i++) {
    setRGBColor(255, 0, 255); // Purple flash
    delay(200);
    setRGBColor(255, 255, 255);
    delay(200);
  }
<<<<<<< Updated upstream
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


=======
  if (systemArmed) {
    setRGBColor(255, 0, 0); // Return to armed red
  } else {
    setRGBColor(0, 0, 255); // Return to disarmed blue
  }
}
>>>>>>> Stashed changes
//need to make rgb better, make the sound integrated into the system