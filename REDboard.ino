#include <Wire.h>

// === Pin Definitions ===
const int soilSensorPin = A0;
const int pwmOutPin     = 3;    // PWM to green Arduino
const int trigPin       = 7;    // Ultrasonic TRIG
const int echoPin       = 10;    // Ultrasonic ECHO
const int ledPin        = 8;    // LED for low reservoir
const int buzzerPin     = 9;    // Buzzer



// === Thresholds ===
const int dryThreshold = 450; // change based on calibration with soil sensor
const int depthToWater = 5;         // in cm (reservoir low alert)
const unsigned long checkInterval = 2000; // Delay between checks (ms

// === State Tracking ===
unsigned long lastCheckTime = 0;

void setup() {
  Serial.begin(9600);

  pinMode(pwmOutPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  
  digitalWrite(ledPin, LOW);   // LED off
}

void loop() {
  unsigned long currentTime = millis();


  // Only check at interval
  if (currentTime - lastCheckTime >= checkInterval) {
    lastCheckTime = currentTime;

    // --- Read Soil Moisture ---
    int soilRaw = analogRead(soilSensorPin);
    int pwmValue = map(soilRaw, 0, 1023, 0, 255);

    analogWrite(pwmOutPin, pwmValue);

    Serial.print("Soil Raw: ");
    Serial.print(soilRaw);
    Serial.print(" → PWM: ");
    Serial.println(pwmValue);

    // --- Read Water Level (Ultrasonic) ---
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(100);
    long duration = pulseIn(echoPin, HIGH);
    float waterHeight = duration * 0.034 / 2.0; //converting the measurement to the distance to the water ,

    Serial.print("Water Height (cm): ");
    Serial.println(waterHeight);


    // --- LED if reservoir low ---
    if (waterHeight > depthToWater) { //when the water height measured is less than the depth to water threshold
      digitalWrite(ledPin, HIGH); // turn LED on when the water resevoir needs to be filled 
      Serial.println("Reservoir Low");
    } else {
      digitalWrite(ledPin, LOW);
      Serial.println("Reservoir OK");
    }

    // --- Trigger buzzer and pump if dry AND water available ---
    if (soilRaw <= dryThreshold && waterHeight >= depthToWater) {

      tone(buzzerPin, 1000, 500);  // Buzz!
      delay(500);
      noTone(buzzerPin); // buzz off

    

    Serial.println("------------------------------");
  }

  // Loop runs constantly, but action only happens every minute
}
}
