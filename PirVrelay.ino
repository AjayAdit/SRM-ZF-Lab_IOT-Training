int pirPin = 2; // Digital pin connected to the PIR sensor
int relayPin = 4 ; // Digital pin connected to the relay module

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(relayPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  int pirState = digitalRead(pirPin);
  if (pirState == HIGH) {
    Serial.println("Motion Detected");
  
    digitalWrite(relayPin, HIGH);

  } else {
    // No motion, turn off the lamp
    digitalWrite(relayPin, LOW);
    Serial.println("No one is there");
  }
}
 