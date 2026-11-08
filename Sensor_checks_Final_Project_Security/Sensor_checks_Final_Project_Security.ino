// Define the sensor and LED pins
const int soundPin = 13; 
const int ledPin = 2;   // Built-in Arduino LED
const int motionPin = 3; //Motion sensor

bool isSearchingForMotion = false;
unsigned long motionWindowStartTime = 0;
const unsigned long searchDuration = 10000; // look detect motion for 10 seconds

void setup() {
  pinMode(soundPin, INPUT); // Set sound sensor as an input
  pinMode(ledPin, OUTPUT);   // Set built-in LED as an output
  pinMode(motionPin, INPUT);

  digitalWrite(ledPin, LOW); // 1. Explicitly start with the LED turned OFF
  Serial.begin(9600);        // Initialize the serial monitor
  Serial.println("System Armed: Listening for suspicious sounds...");
}

void loop() {
  
  // STATE 1: System is purely listening for sound
  if (!isSearchingForMotion) {
    if (digitalRead(soundPin) == LOW) {
      isSearchingForMotion = true;
      motionWindowStartTime = millis(); // Start the 10-second countdown clock
      Serial.println("⚠️ SOUND DETECTED! Activating motion sensor scan...");
    }
  }
  
  // STATE 2: Sound was heard, now actively hunting for movement
  else {
    int motionState = digitalRead(motionPin);
    
    if (motionState == HIGH) {
      // Intrudor caught! Turn on the alarm LED
      digitalWrite(ledPin, HIGH); 
      Serial.println("🚨 ALERT: MOTION DETECTED! Intruder confirmed.");
    }
    
    // Check if the 10-second search window has expired
    if (millis() - motionWindowStartTime >= searchDuration) {
      isSearchingForMotion = false; // Reset the system switch
      digitalWrite(ledPin, LOW);    // Turn off the alarm LED
      Serial.println("🔄 Area clear. Motion window expired. Re-arming sound sensor...");
    }
  }
  
}