const int pumpPin = 10;  // Pin connected to relay module controlling the pump

void setup() {
  pinMode(pumpPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("🚿 Pump test starting...");
}

void loop() {
  // Turn pump ON
  digitalWrite(pumpPin, HIGH);
  Serial.println("💧 Pump ON");
  delay(5000);  // Pump on for 5 seconds

  // Turn pump OFF
  digitalWrite(pumpPin, LOW);
  Serial.println("💤 Pump OFF");
  delay(5000);  // Pump off for 5 seconds
}
