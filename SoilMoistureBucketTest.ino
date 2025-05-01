// === CONFIGURATION ===
const bool testMode = true;  // <-- set to true to force LED & buzzer ON for testing

// === PIN DEFINITIONS ===
const int moisturePin = A0;
const int trigPin = 7;
const int echoPin = 10;
const int ledPin = 8;
const int buzzerPin = 9;
const int PWM = 3;

// === THRESHOLDS ===
const int dryThreshold = 60;           // based on your calibration
const int minWaterHeight = 6;          // cm
const int measurementInterval = 6000; // 60 seconds

// === FUNCTIONS ===
long readUltrasonicCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void playBuzzer() {
  tone(buzzerPin, 1000, 500);  // 1 kHz for 500 ms
  delay(500);
  noTone(buzzerPin);
}

// === SETUP ===
void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
}

// === LOOP ===
void loop() {
  if (testMode) {
    Serial.println("🧪 TEST MODE ACTIVE: LED and buzzer will turn ON every loop");
    digitalWrite(ledPin, HIGH);
    playBuzzer();
    delay(measurementInterval);
    analogWrite(PWM, 255);
    //return;
  }

  // Read soil moisture
  int moistureValue = analogRead(moisturePin);
  Serial.print("Soil Moisture Reading: ");
  Serial.print(moistureValue);
  if (moistureValue <= dryThreshold) {
    Serial.println(" → Dry");
  } else {
    Serial.println(" → Moist");
  }

  // Read ultrasonic sensor
  long waterHeight = readUltrasonicCM();
  Serial.print("Water Height (cm): ");
  Serial.println(waterHeight);

  // Water low = LED ON
  if (waterHeight > minWaterHeight) {
    digitalWrite(ledPin, HIGH);
    Serial.println("⚠️ Bucket water is LOW — LED ON");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("✅ Bucket water is OK — LED OFF");
  }

  // Trigger buzzer if dry + water is available
  if (moistureValue <= dryThreshold && waterHeight >= minWaterHeight) {
    Serial.println("💧 Soil is dry AND water is available — Buzzer ON");
    playBuzzer();
  } else {
    Serial.println("🔁 No watering needed or water unavailable.");
  }

  delay(measurementInterval);
}
