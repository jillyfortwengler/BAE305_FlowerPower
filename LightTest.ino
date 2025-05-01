const int relayPin = 7;

void setup() {
  pinMode(relayPin, OUTPUT);
}

void loop() {
  digitalWrite(relayPin, HIGH); // Light ON
 delay(2000);                  // Wait 2 seconds
 // digitalWrite(relayPin, LOW);  // Light OFF
  //delay(2000);                  // Wait 2 seconds
}

