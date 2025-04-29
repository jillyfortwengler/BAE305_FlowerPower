# Flower Power Plant Staycation

Group Members: Brynn Cooper, Jillian Fortwengler, Lauren Kargas

## Summary

The Flower Power Plant Staycation was thoughtfully designed with busy college students in mind, especially those who love their plants but may not always have the time to properly care for them. This innovative system features a fully automated watering and lighting setup that users can customize based on their plant’s specific needs. A SparkFun Soil Moisture Sensor continuously monitors soil moisture, triggering watering via pump only when necessary to conserve water and prevent overwatering. An ultrasonic sensor monitors the water reservoir level and activates an LED indicator when a refill is needed. By automating essential plant care tasks, the Flower Power Plant Staycation helps students maintain healthy, thriving plants with minimal daily effort.


## Design Description

### Materials and Planning
We began by outlining the materials needed for the project. Key components were carefully selected based on their performance and compatibility:
- UV Grow Light to promote optimal plant growth
- SparkFun Inventor's Kit
- SparkFun Soil Sensor
- Water pump motor capable of extended use
- Circuit components: RTC (Real-Time Clock) module and two relays

![image](https://github.com/user-attachments/assets/41541d39-87c2-4b2c-950a-eb7c08c0e04e)

**_Figure 1: Bill of Materials_**


### Part 1: Automated Watering System

**Motor and Soil Sensor Control:**

The motor is connected to a relay, which switches it on and off based on soil moisture readings. A SparkFun Soil Moisture Sensor detects when watering is needed. A piezo buzzer sounds during watering to alert the user.

![image](https://github.com/user-attachments/assets/1b75d73b-1a72-480d-930b-cf0495b2d304)

**_Figure 2: Motor and Soil Sensor Circuit Design_**


**Reservoir Monitoring:**

An ultrasonic sensor detects the water level in the reservoir. If the water level drops too low, an LED light illuminates to signal a refill is needed.

![image](https://github.com/user-attachments/assets/13b4dcab-2bd2-4770-8004-b3d2e1141c25)

**_Figure 3: Ultrasonic Sensor Circuit Design_**

**LCD Display Integration:**

An LCD screen displays the real-time clock and soil moisture percentage.

![image](https://github.com/user-attachments/assets/41dd840b-e424-466f-9f6b-9a21830703e1)

**_Figure 4: LCD Screen Circuit Design_**


### Part 2: Automated Lighting System
**Real-Time Clock:**

The light bulb is connected to a relay module to control when the light is on and off. This relay controls the light on a schedule based on real time using the Real-Time Clock Module. 

![image](https://github.com/user-attachments/assets/67652321-0dcf-4e61-8ada-a5317d8eecd0)

**_Figure 5: Light Bulb and RTC Circuit Design_**


**Soil Moisture Signal**
To connect the soil moisture readings to the LCD, the signal must be converted from analog to digital across the Arduinos. This is done by filtering the signal through a capacitor and resistor to clean the PWM signal. 

![image](https://github.com/user-attachments/assets/4e8bf86e-3239-4440-9d50-e741bc168066)

**_Figure 6: Multi-Arduino Integration Circuit Design_**

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
#1.

App flowchart


## Testing Description

We thoroughly tested each component individually and then as a complete system. The testing setup included:
•	Soil Sensor Calibration:
Using a Digital Multimeter to measure voltage outputs across the sensor to test connection. 
•	RTC set up and testing:
Verified and adjusted time using the RTC library from Arduino IDE. 
•	Ultrasonic Sensor testing:
Confirmed distance measurement accuracy for reservoir monitoring.
•	Pump Activation test:
Confirmed relay control and correct moisture threshold activation. 
•	Grow Light Testing:
Verified that the relay correctly switched the light on and off according to the RTC settings. 
•	RTC Adjustments:
Fine-tuned timing to align lighting schedule and moisture readings 

Equipment used:
•	Digital Multimeter (voltage output reading)
•	Arduino IDE Serial Monitor for sensor output confirmation

## Design Decision Discussion

This design is an excellent reference system for similar automated plant care projects. All design decisions were made to ensure compatibility and reliability. 

Our first design decision was the use of this box. The box made it to where we could easily contain all the components in designated areas for easy access. Within this box, we created a top portion to house the light circuitry and a bottom portion to house the water automation circuitry and water reservoir. Within this box, a roof with hinges was attached to allow easy access to the top circuit, plus both of the wooden floorings can be detached if desired. This setup allows for future expandability if needed, as there is ample room to add more components. 

The next design decision was to separate each of the coding sections by what component it was coding for. For example, there was a specific soil moisture sensor code, an LCD screen code, and a light system code. This decision allows organization within both the Green and Red Board Code, so if any code needs to be edited or expanded on, it is an easy process.

For the next design decision, we created an app to allow the user to automate the watering and light system with ease. Within this app, you can change when you want the light to be on/ off based on real time. For example, you would input that you want the light to be on from 8 am- 8 pm. Thinking about the user, we also included plant health tracking code, nutrient intake guide, and refill reminder notifications.

To adapt the automated lighting and watering system, additional components can be added like WiFi capability or enhanced nutrient monitoring. 

## Test Results Discussion

The system reliably automates watering and lighting for a small to medium-sized houseplant. First, it successfully prevents overwatering and a lack of watering by saturating the plant only when needed based on a soil moisture sensor. Furthermore, it reminds the user when the water reservoir is low by lighting up a red LED seen on the front of the box. Because of this, the water doesn’t run dry without the user knowing. Moreover, the plant system provides real-time status updates through both the LCD screen on the front of the box and the mobile app. Other features include a speaker to indicate that the pump is watering the plant, and app components such as a nutrient guide and plant health guide.

This plant system was designed for the audience of plant enthusiasts who struggle to keep their plants alive, don’t have a sufficient amount of sunlight to allow the plant to thrive, or are too busy to remember to water their plants. Keeping this in mind, we designed this system to work best in a house, apartment, dorm room, office, or small indoor garden. The peak performance of this box is on a flat surface, next to a wall plug, and in a moderate climate room (65 - 75°F). This system works without fail to water and provide light to a plant for as long as the user refills the water reservoir. 

The limitations of this system are as follows. The current version requires manual reservoir refilling (no auto-refill system). Furthermore, this system is designed for a single medium plant or 2-3 small plants. Finally, the plant automation system requires a wall plug for the light system to work properly.

# Testing Results

All of the following components were tested both individually and with the integrated system to ensure full operational capability.
### 1. Soil moisture sensing and pump activation
The SparkFun Soil Moisture Sensor accurately detected soil dryness and triggered the water pump through a relay when readings fell below the calibrated moisture threshold. The system consistently avoided overwatering, and pump activation was confirmed with both water flow and buzzer alerts during each test cycle. To test the pump system, we used the following code to turn the system on and off to test its performance:
```c++
const int pumpPin = 13;  // Pin connected to relay module controlling the pump

void setup() {
  pinMode(pumpPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Pump test starting...");
}

void loop() {
  // Turn pump ON
  digitalWrite(pumpPin, HIGH);
  Serial.println("Pump ON");
  delay(5000);  // Pump on for 5 seconds

  // Turn pump OFF
  digitalWrite(pumpPin, LOW);
  Serial.println("Pump OFF");
  delay(5000);  // Pump off for 5 seconds
}
```
To test the soil moisture sensor, we used the following code to turn the light on from 8 am – 8 pm by connecting it to the RTC:
```c++
const int lightOnHour = 8;
const int lightOffHour = 20;
if ((hour >= lightOnHour) && (hour < lightOffHour)) {
    digitalWrite(relayPin, HIGH);  // Light ON
    Serial.println(hour);
    Serial.println("Light ON");} 
    else {
    digitalWrite(relayPin, LOW);   // Light OFF
    Serial.println("Light OFF");
  }
```

### 2. Water Reservoir Monitoring and LED Alert

The ultrasonic sensor accurately measured water depth and illuminated the LED alert light when water levels dropped below the 2 cm threshold. This test ensured users receive timely visual notifications to refill the reservoir and avoid pump damage or plant dehydration. We used the following code to test the motor by turning it on and off:
```c++
const int pumpRelayPin = 12; // Digital pin connected to relay IN
void setup() {
  pinMode(pumpRelayPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Pump Test Starting...");
}
void loop() {
  Serial.println("Pump ON");
  digitalWrite(pumpRelayPin, HIGH); // Turn pump ON
  delay(3000);                       // ON for 3 seconds
  Serial.println("Pump OFF");
  digitalWrite(pumpRelayPin, LOW);  // Turn pump OFF
  delay(3000);                       // OFF for 3 seconds
}
```
We used the following code to test that the LED could turn on and off:
```c++
const int ledPin = 8; // Digital pin connected to LED

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("LED Test Starting...");
}

void loop() {
  Serial.println("LED ON");
  digitalWrite(ledPin, HIGH); // Turn LED ON
  delay(1000);                // ON for 1 second

  Serial.println("LED OFF");
  digitalWrite(ledPin, LOW);  // Turn LED OFF
  delay(1000);                // OFF for 1 second
}
```

### 3. Real-Time Clock (RTC) Functionality and Light Scheduling

The RTC module was tested to verify accurate timekeeping and light control. Grow light activation and deactivation reliably followed the programmed schedule (8 am – 8 pm) with multiple successful cycles confirming stable relay switching based on RTC time. We ran into some issues getting the RTC to connect with the light system but switched the light circuit to be on the same breadboard as the RTC and then it worked perfectly. The code we used to test the RTC individually is as follows:
```c++
#include <Wire.h>
#include "RTClib.h"

RTC_PCF8523 rtc;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    while (1);
  }

  // OPTIONAL: Set the RTC once, then comment this out
  // rtc.adjust(DateTime(2025, 4, 29, 14, 30, 0));  // Year, Month, Day, Hour, Minute, Second

  Serial.println("RTC is running...");
}

void loop() {
  DateTime now = rtc.now();

  Serial.print("Current Time: ");
  Serial.print(now.year());
  Serial.print('/');
  Serial.print(now.month());
  Serial.print('/');
  Serial.print(now.day());
  Serial.print(" ");
  Serial.print(now.hour());
  Serial.print(':');
  Serial.print(now.minute());
  Serial.print(':');
  Serial.println(now.second());

  delay(1000);
}
```
### 4. LCD Display Output
The LCD screen displayed live data from the sensors, including soil moisture values and the time of the day through the RTC. It updated correctly in response to sensor changes, confirming proper communication between the Arduino and display module. The code we used to test the LCD individually is as follows:
```c++
#include <LiquidCrystal.h>

// LCD pins: RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  lcd.begin(16, 2); // 16 columns, 2 rows
  lcd.print("LCD Test Start");
  delay(2000); // Display for 2 seconds
  lcd.clear();
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Line 1: Hello!");

  lcd.setCursor(0, 1);
  lcd.print("Line 2: Test OK");

  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Counter Test:");
  for (int i = 0; i <= 9; i++) {
    lcd.setCursor(13, 1); // Right-aligned counter
    lcd.print(i);
    delay(1000);
  }

  lcd.clear(); // Reset display
}
```
The soil moisture sensor, pump, ultrasonic sensor, LED Alert, RTC, and LCD Display were all important components in our circuits that we had to test both individually and in the integrated system. Testing validated the success of the Flower Power Plant Staycation prototype and provided a solid foundation for future development and improvements. 


## Conclusion

The Flower Power Plant Staycation successfully automates essential plant care tasks, providing an easy, reliable way for busy students to keep their plants alive and thriving. With customizable watering and lighting settings, built-in reminders, and real-time monitoring, the system offers peace of mind and healthier plants with minimal daily maintenance. Future improvements could further expand its functionality, but even in its current form, the Flower Power system provides a valuable, scalable solution for modern plant lovers.

