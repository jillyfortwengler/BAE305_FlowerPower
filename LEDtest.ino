void setup() {
  pinMode(8, OUTPUT);
}

void loop() {
  digitalWrite(8, HIGH); // Turn LED ON
  delay(1000);
  digitalWrite(8, LOW);  // Turn LED OFF
  delay(1000);
}