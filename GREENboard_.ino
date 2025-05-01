#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>

// --- RTC + LCD Setup ---
RTC_PCF8523 rtc;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // RS, EN, D4, D5, D6, D7

// --- Pins ---
const int soilInputPin = A1;
const int relayPin = 7;  // Relay controlling grow light

// --- Light Schedule (24hr format) ---
const int lightOnHour = 8;
const int lightOffHour = 20;
 

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // LCD
  lcd.begin(16, 2);
  lcd.print("Starting...");
  delay(1000);
  lcd.clear();

  // Relay
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);  // Light OFF at start

  // RTC
  if (!rtc.begin()) {
    lcd.print("RTC not found!");
    Serial.println("RTC not found!");
    while (1);
  }

  // Set time once (optional, then comment out)
  // rtc.adjust(DateTime(2025, 4, 21, 15, 45, 0));
}

void loop() {
  DateTime now = rtc.now();
  int hour = now.hour();
  int minute = now.minute();


  // --- Light Control (Relay) ---
  if ((hour >= lightOnHour) && (hour < lightOffHour)) {
    digitalWrite(relayPin, HIGH);  // Light ON
    Serial.println(hour);
    Serial.println("💡 Light ON");} 
    else {
    digitalWrite(relayPin, LOW);   // Light OFF
    Serial.println("🌙 Light OFF");
  }

  // --- Soil Moisture ---
  int analogValue = analogRead(soilInputPin);  // 0-1023
  int moisturePercent = map(analogValue, 50, 1023, 0, 100);  // Calibrated

  Serial.println(analogValue);

  // --- LCD Display ---
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  if (hour < 10) lcd.print("0");
  lcd.print(hour);
  lcd.print(":");
  if (minute < 10) lcd.print("0");
  lcd.print(minute);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("Soil: ");
  lcd.print(moisturePercent);
  lcd.print("%     ");

 

  delay(1000);  // Update every second
}
