# Flower Power Plant Staycation

## Summary

The Flower Power Plant Staycation was thoughtfully designed with busy college students in mind, especially those who love their plants but may not always have the time to properly care for them. This innovative system features a fully automated watering and lighting setup that users can customize based on their plant’s specific needs. A SparkFun Soil Moisture Sensor continuously monitors soil moisture, triggering watering via pump only when necessary to conserve water and prevent overwatering. An ultrasonic sensor monitors the water reservoir level and activates an LED indicator when a refill is needed. By automating essential plant care tasks, the Flower Power Plant Staycation helps students maintain healthy, thriving plants with minimal daily effort.

---

## Design Description

### Materials and Planning
We began by outlining the materials needed for the project. Key components were carefully selected based on their performance and compatibility:
- UV Grow Light to promote optimal plant growth
- SparkFun Inventor's Kit
- SparkFun Soil Sensor
- Water pump motor capable of extended use
- Circuit components: RTC (Real-Time Clock) module and two relays

**_Figure 1: Bill of Materials_**

Picture 1873942076, Picture

---

### Part 1: Automated Watering System

**Motor and Soil Sensor Control:**

The motor is connected to a relay, which switches it on and off based on soil moisture readings. A SparkFun Soil Moisture Sensor detects when watering is needed. A piezo buzzer sounds during watering to alert the user.

**_Figure 2: Motor and Soil Sensor Circuit Design_**

Picture 551921340, Picture

**Reservoir Monitoring:**

An ultrasonic sensor detects the water level in the reservoir. If the water level drops too low, an LED light illuminates to signal a refill is needed.

**_Figure 3: Ultrasonic Sensor Circuit Design_**

Picture 906056473, Picture

**LCD Display Integration:**

An LCD screen displays the real-time clock and soil moisture percentage.

**_Figure 4: LCD Screen Circuit Design_4**

Picture 833415367, Picture

### Part 2: Automated Lighting System
**Real-Time Clock:**

The light bulb is connected to a relay module to control when the light is on and off. This relay controls the light on a schedule based on real time using the Real-Time Clock Module. 

**_Figure 5: Light Bulb and RTC Circuit Design_ **

Picture 623274102, Picture

**Soil Moisture Signal**
To connect the soil moisture readings to the LCD, the signal must be converted from analog to digital across the Arduinos. This is done by filtering the signal through a capacitor and resistor to clean the PWM signal. 

**_Figure 6: Multi-Arduino Integration Circuit Design_**
Picture 1777663324, Picture

### Part 3: Code
## 1. Red Board Code 

This code includes the soil moisture sensor, ultrasonic sensor, and the indicator LED. Within this code, it is important to define pin definitions and only use one void loop function. This code is outlined below. 

```c++ 

//code controls soil sensor, pump to activate based on soil, buzzer beeps when watering is complete, and LED turns on to signal refill reminder.  

// === Libraries ===

#include <Wire.h> 


// === Pin Definitions === 

const int soilSensorPin = A0; 
const int pwmOutPin     = 3;    // PWM to green Arduino 
const int pumpPin       = 12;   // Relay for pump 
const int trigPin       = 7;    // Ultrasonic TRIG 
const int echoPin       = 6;    // Ultrasonic ECHO 
const int ledPin        = 8;    // LED for low reservoir 
const int buzzerPin     = 9;    // Buzzer 

// === Thresholds === 

const int dryThreshold = 480;          // Adjust for calibration 
const int depthToWater = 2.75;         // in cm (reservoir low alert) 
const unsigned long wateringTime = 2000;  // Pump ON time (ms) 
const unsigned long checkInterval = 2000; // Delay between checks (ms 

// === State Tracking === 

unsigned long lastCheckTime = 0; 

void setup() { 

  Serial.begin(9600); 

  pinMode(pwmOutPin, OUTPUT); 
  pinMode(pumpPin, OUTPUT); 
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(ledPin, OUTPUT); 
  pinMode(buzzerPin, OUTPUT); 

  digitalWrite(pumpPin, LOW);  // Ensure pump is off 
  digitalWrite(ledPin, LOW);   // LED off 

} 

void loop() { 

  unsigned long currentTime = millis(); 

  // Only check at interval 

  if (currentTime - lastCheckTime >= checkInterval) { 
    lastCheckTime = currentTime; 

    // --- Read Soil Moisture --- 

    int soilRaw = analogRead(soilSensorPin); 
    int pwmValue = map(soilRaw, 0, 532, 0, 255); 

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
    long duration = pulseIn(echoPin, HIGH); 

    float waterHeight = duration * 0.034 / 2.0; //converting the measurement to the distance to the water , 

    Serial.print("Water Height (cm): "); 
    Serial.println(waterHeight); 

    // --- LED if reservoir low --- 

    if (waterHeight < depthToWater) { //when the water height measured is less than the depth to water threshold 
     digitalWrite(ledPin, HIGH); // turn LED on when the water resevoir needs to be filled  
      Serial.println("Reservoir Low"); 

    } else { 

      digitalWrite(ledPin, LOW); 
      Serial.println("Reservoir OK"); 
    }

    // --- Trigger buzzer and pump if dry AND water available --- 

    if (soilRaw <= dryThreshold && waterHeight >= depthToWater) { 

      Serial.println("Now watering"); 
      tone(buzzerPin, 1000, 500);  // Buzz! 
      delay(500); 
      noTone(buzzerPin); // buzz off 

      digitalWrite(pumpPin, HIGH); 
      delay(wateringTime); 
      digitalWrite(pumpPin, LOW); 
      Serial.println("Pump OFF"); 

    } else { 
      Serial.println("No watering needed or water unavailable."); 

    } 
    Serial.println("------------------------------"); 

  } 
  // Loop runs constantly, but action only happens every minute 

} 

``` 

 

## 2. Green Board Code 

This code includes the Light Bulb, the Real-Time Module, and the LCD Screen. Within this code, it is important to download the libraries Wire.h, RTClib.h, and LiquidCrystal.h. These can be found via the Library Manager tab in Arduino IDE. The Green Board Code is outlined below. 

```c++ 

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
    Serial.println("Light ON");}  

    else { 
    digitalWrite(relayPin, LOW);   // Light OFF 
    Serial.println("Light OFF"); 

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

 

``` 

## Part 4 – The Box 

## Part 5 - The App
 
---

## Testing Description

Testing setup and procedures included:
- **Soil Sensor Calibration:** Using a Digital Multimeter to measure voltage outputs across different soil conditions.
- **RTC Setup and Testing:** Verifying and adjusting time with the RTC library.
- **Ultrasonic Sensor Testing:** Confirming distance measurement accuracy.
- **Pump Activation Test:** Testing relay control and moisture threshold activation.
- **Grow Light Testing:** Verifying correct relay switch based on RTC schedule.
- **RTC Adjustments:** Fine-tuning timing for lighting and moisture syncing.

**Equipment used:**
- Digital Multimeter
- Arduino IDE Serial Monitor

---

## Design Decision Discussion

This design serves as an excellent reference for similar automated plant care projects:
- Modular wiring for future scalability
- RTC and relay modules for flexible scheduling and automation
- Easy code modification for different plant species or additional features

Future modifications could include:
- WiFi or Bluetooth integration
- Mobile app control
- Expanded multi-plant support
- Nutrient intake tracking

---

## Test Results Discussion

**Capabilities:**
- Soil moisture sensing and pump activation
- Water reservoir level monitoring
- Real-time display updates
- Grow light scheduling

**Limitations:**
- Manual reservoir refilling required
- Single plant system (expandable with more hardware)
- No remote access yet

**Ideal Usage:**
- Apartments, dorms, small offices, or indoor gardens.

---

## Testing Results

All major functions were tested and verified:
- Soil sensor calibrated accurately
- RTC and light schedule functioned correctly
- Ultrasonic sensor reliably detected low reservoir levels
- LCD displayed time and moisture readings correctly

---

## Conclusion

The **Flower Power Plant Staycation** provides an accessible, automated solution for maintaining healthy plants with minimal daily effort. By integrating smart watering, light scheduling, and refill reminders, this system empowers busy individuals to keep their plants thriving even during their most hectic schedules. With opportunities for future improvements, the Flower Power project is a strong foundation for next-generation automated plant care systems.

---

# 🌿 Flower Power Project
