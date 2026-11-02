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
  while (!huskylens.begin(Wire)) {
    Serial.println(F("Your virtual dog is hungry foo"));
    delay(1000);
  }
  Serial.println(F("Your virtual pet is starting up ..."));
  
  setRGBColor(0, 0, 255); // Blue means Standby/Disarmed
}

void loop() {
  // 1. Check On/Off Button Switch
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
      }
    }
  }

  delay(100);
}

// --- Helper Functions ---

void setRGBColor(int red, int green, int blue) {
  analogWrite(redpin, red);
  analogWrite(greenpin, green);
  analogWrite(bluepin, blue);
}

void triggerAlarm() {
  // Flash RGB LED purple/white or sound buzzer/lock
  for (int i = 0; i < 3; i++) {
    setRGBColor(255, 0, 255); // Purple flash
    delay(200);
    setRGBColor(255, 255, 255);
    delay(200);
  }
  if (systemArmed) {
    setRGBColor(255, 0, 0); // Return to armed red
  } else {
    setRGBColor(0, 0, 255); // Return to disarmed blue
  }
}
//need to make rgb better, make the sound integrated into the system