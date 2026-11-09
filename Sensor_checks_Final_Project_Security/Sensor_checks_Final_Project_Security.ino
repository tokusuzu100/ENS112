// Define the sensor and LED pins
const int soundPin = 13; 
const int ledPin = 2;   // yellow to show intruder
const int motionPin = 5; // Motion sensor
const int buttonPin = 3; 
const int piezoPin = A0;
const int redPin = 4; // to show system is armed

// knocklock stuff
const int knockthreshold = 40;
const int secretknockcount = 3; // require taps
const unsigned long knockmaxdelay = 1200; // time between taps allowed

int currentknockcount = 0;
unsigned long lastknocktime = 0;

bool isArmed = false; // this is master "toggle" activated by knocklock
bool isSearchingForMotion = false; // secondary trigger when sound is detected 
unsigned long motionWindowStartTime = 0;
const unsigned long searchDuration = 10000; // look detect motion for 10 seconds

void setup() {
  pinMode(soundPin, INPUT); // Set sound sensor as an input
  pinMode(ledPin, OUTPUT);   // Set yellow LED as an output
  pinMode(motionPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP); 
  pinMode(redPin, OUTPUT);

  digitalWrite(ledPin, LOW); // Explicitly start with the LED turned OFF
  digitalWrite(redPin, LOW);

  Serial.begin(9600);        // Initialize the serial monitor
  Serial.println("System Initialized. Status: DISARMED. Press button to Arm.");
}

void loop() {
  // KNOCKLOCK
  if (digitalRead(buttonPin) == LOW && !isArmed){
    isArmed = true;
    isSearchingForMotion = false;

    digitalWrite(redPin, HIGH); 
    Serial.println("[LOCKED] SYSTEM ARMED: ACTIVE DEFENSE ENABLED!");
    delay(500);
  } 

  // to unlock knocklock
  int piezoReading = analogRead(piezoPin);

  if (piezoReading >= knockthreshold) {
    unsigned long currentTime = millis();

    if (currentTime - lastknocktime > knockmaxdelay && currentknockcount > 0){
      currentknockcount = 0;
      Serial.println("TIMEOUT: Too slow! Knock pattern reset.");
    }
    
    currentknockcount++;
    lastknocktime = currentTime; 
    Serial.print("KNOCK: Tap registered! Count: ");
    Serial.println(currentknockcount); 
    
    if (currentknockcount >= secretknockcount) {
      if (isArmed){
        isArmed = false;
        digitalWrite(redPin, LOW);
        digitalWrite(ledPin, LOW);
        Serial.println("[UNLOCKED] SYSTEM DISARMED: Access granted. System Down.");
      }
      currentknockcount = 0;
    }

    delay(200); 
  }

  if (currentknockcount > 0 && (millis() - lastknocktime > knockmaxdelay)){ 
    currentknockcount = 0;
    Serial.println("SYSTEM: Pattern timed out. Resetting count."); 
    
    // flash red LED showing reset of code
    for(int i=0; i<3; i++){
      digitalWrite(redPin, HIGH); delay(150);
      digitalWrite(redPin, LOW); delay(150);
    }
  }
  
  // security logic
  if (isArmed) {
    // STATE 1: System is purely listening for sound
    if (!isSearchingForMotion) {
      if (digitalRead(soundPin) == LOW) {
        isSearchingForMotion = true;
        motionWindowStartTime = millis(); // Start the 10-second countdown clock
        Serial.println("WARNING: SOUND DETECTED! Activating motion sensor scan...");
      }
    }
  
    // STATE 2: Sound was heard, now actively hunting for movement
    else {
      if (digitalRead(motionPin) == HIGH) {
        // Intruder caught! Turn on the alarm LED
        digitalWrite(ledPin, HIGH); 
        Serial.println("ALARM: MOTION DETECTED! Intruder confirmed.");
      }
    
      // Check if the 10-second search window has expired
      if (millis() - motionWindowStartTime >= searchDuration) {
        isSearchingForMotion = false; // Reset the system switch
        digitalWrite(ledPin, LOW);    // Turn off the alarm LED
        Serial.println("SYSTEM: Area clear. Motion window expired. Re-arming sound sensor...");
      }
    }
  }
}
