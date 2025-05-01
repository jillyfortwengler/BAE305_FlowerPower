#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>

// --- Constants & Global Setup ---
RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const int soilPin = A0;
//RTC_DS3231 rtc;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // RS, EN, D4, D5, D6, D7



void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  Wire.begin();
  
  float drift = 43; // seconds plus or minus over oservation period - set to 0 to cancel previous calibration.
  float period_sec = (7 * 86400);  // total obsevation period in seconds (86400 = seconds in 1 day:  7 days = (7 * 86400) seconds )
  float deviation_ppm = (drift / period_sec * 1000000); //  deviation in parts per million (μs)
  float drift_unit = 4.34; // use with offset mode PCF8523_TwoHours
  // float drift_unit = 4.069; //For corrections every min the drift_unit is 4.069 ppm (use with offset mode PCF8523_OneMinute)
  int offset = round(deviation_ppm / drift_unit);

  if (!rtc.begin()) {
    lcd.print("RTC not found!");
    while (1);
  }

  // Optional: Set the time manually once
  // rtc.adjust(DateTime(2025, 4, 21, 15, 24, 0));  

  lcd.clear();
  lcd.print("RTC + Soil Ready");
  delay(1500);

  // Print initial date/time to serial
  DateTime now = rtc.now();
  Serial.print(now.year()); Serial.print('/');
  Serial.print(now.month()); Serial.print('/');
  Serial.print(now.day()); Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour()); Serial.print(':');
  Serial.print(now.minute()); Serial.print(':');
  Serial.println(now.second());
}

void loop() {
  DateTime now = rtc.now();  // ✅ Get fresh time each loop

  // --- Soil Moisture ---
  int rawSoil = analogRead(soilPin);
  int moisturePercent = map(rawSoil, 1023, 300, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);
Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
  // --- LCD Display ---
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  if (now.hour() < 10) lcd.print("0");
  lcd.print(now.hour());
  lcd.print(":");
  if (now.minute() < 10) lcd.print("0");
  lcd.print(now.minute());

  lcd.setCursor(0, 1);
  lcd.print("Soil: ");
  lcd.print(moisturePercent);
  lcd.print("%   ");

  // --- Serial Output ---
  Serial.print("Time: ");
  Serial.print(now.hour()); Serial.print(":");
  Serial.print(now.minute()); Serial.print(":");
  Serial.print(now.second());
  Serial.print(" | Soil: ");
  Serial.print(moisturePercent); Serial.println("%");

  delay(1000);
}
