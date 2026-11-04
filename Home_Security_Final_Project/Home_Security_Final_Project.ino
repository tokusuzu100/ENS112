#include <Servo.h>
#include <Wire.h>
#include "HUSKYLENS.h"
#include <DHT.h>

// Hardware pins
const int soundpin = 2; // Reserved for a future buzzer/alarm output
const int motionsensor = 13;
const int noisesensor = 10;
const int temp_humiditysensor = 20;
const int redpin = 0;
const int greenpin = 0;
const int bluepin = 0;
const int buttonswitch = 3;

#define DHTTYPE DHT11

Servo myServo;
DHT dht(temp_humiditysensor, DHTTYPE);
HUSKYLENS huskylens;

bool systemArmed = false;
bool lastButtonState = HIGH;
const float Regular_Room_Temp = 77.0;

void setRGBColor(int red, int green, int blue);
void triggerAlarm();
void activateHuskyLensScan();

void setup() {
  Serial.begin(9600);
  pinMode(motionsensor, INPUT);
  pinMode(noisesensor, INPUT);
  pinMode(buttonswitch, INPUT_PULLUP);

  dht.begin();
  Wire.begin();
  myServo.attach(9);
  myServo.write(0);

  while (!huskylens.begin(Wire)) {
    Serial.println(F("HuskyLens not detected; check its wiring."));
    delay(1000);
  }
  Serial.println(F("Security system ready."));
  setRGBColor(0, 0, 255); // Blue means disarmed
}

void loop() {
  const bool buttonState = digitalRead(buttonswitch);
  if (buttonState == LOW && lastButtonState == HIGH) {
    systemArmed = !systemArmed;
    delay(200); // Debounce

    if (systemArmed) {
      myServo.write(90);
      Serial.println(F("System armed; lock active."));
      setRGBColor(255, 0, 0);
    } else {
      myServo.write(0);
      Serial.println(F("System disarmed; lock released."));
      setRGBColor(0, 0, 255);
    }
  }
  lastButtonState = buttonState;

  if (systemArmed) {
    const float tempF = dht.readTemperature(true);
    const int soundDetected = digitalRead(noisesensor);
    const int motionDetected = digitalRead(motionsensor);

    if (soundDetected == HIGH && motionDetected == HIGH) {
      Serial.println(F("Sound and motion detected."));
      triggerAlarm();
      activateHuskyLensScan();
    } else if (!isnan(tempF) && tempF > Regular_Room_Temp) {
      Serial.print(F("High temperature: "));
      Serial.print(tempF);
      Serial.println(F(" F. Starting HuskyLens scan."));
      activateHuskyLensScan();
    }
  }

  delay(200);
}

void setRGBColor(int red, int green, int blue) {
  analogWrite(redpin, red);
  analogWrite(greenpin, green);
  analogWrite(bluepin, blue);
}

void triggerAlarm() {
  for (int i = 0; i < 3; i++) {
    setRGBColor(255, 0, 255);
    delay(200);
    setRGBColor(255, 255, 255);
    delay(200);
  }
  setRGBColor(255, 0, 0); // Return to armed red
}

void activateHuskyLensScan() {
  if (huskylens.request()) {
    if (huskylens.isLearned()) {
      Serial.println(F("HuskyLens recognized a learned face/object."));
    } else {
      Serial.println(F("HuskyLens detected something not learned yet."));
    }
  } else {
    Serial.println(F("HuskyLens request failed; check wiring."));
  }
}
