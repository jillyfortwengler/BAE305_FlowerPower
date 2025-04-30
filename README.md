# Flower Power Plant Staycation
theme: jekyll-theme-leap-day

Group Members: Brynn Cooper, Jillian Fortwengler, Lauren Kargas

# Summary

The Flower Power Plant Staycation was thoughtfully designed with busy college students in mind, especially those who love their plants but may not always have the time to properly care for them. This innovative system features a fully automated watering and lighting setup that users can customize based on their plant’s specific needs. A SparkFun Soil Moisture Sensor continuously monitors soil moisture, triggering watering via pump only when necessary to conserve water and prevent overwatering. An ultrasonic sensor monitors the water reservoir level and activates an LED indicator when a refill is needed. By automating essential plant care tasks, the Flower Power Plant Staycation helps students maintain healthy, thriving plants with minimal daily effort.


# Design Description

## Materials and Planning

1. First, we created a Bill of Materials (BoM) needed for this project. We selected the best components based on their performance and capability for the needs of the project.
   
*	A UV Grow Light to promote optimal plant growth
*	SparkFun Soil Sensor
*	A LED bulb extension cord with a switch
*	A reliable water pump motor capable of extended use
*	A tube for the water pump
*	Circuit components: Real-Time Clock (RTC), two relays, LCD screen, 

![image](https://github.com/user-attachments/assets/41541d39-87c2-4b2c-950a-eb7c08c0e04e)

**_Figure 1: Bill of Materials_**


### Part 1: Automated Watering System

**Motor and Soil Sensor Control:**

The motor is connected to a relay, which switches the pump on and off based on the soil moisture reading. To create the automated light system, attach a light bulb to a relay in order to control when it turned on and off. Furthermore, to make this light system based on real time, incorporate a Real-Time Module (RTC). This circuit is shown in Figure 2.

![image](https://github.com/user-attachments/assets/9b303479-a21b-4afa-8086-95b45c993d53)

**_Figure 2: Motor and Light Circuit Design_**


**Reservoir Monitoring:**

Next, use an ultrasonic sensor to detect when the reservoir is low. If the water level is low, it lights up an LED. Furthermore, a SparkFun Soil Moisture Sensor detects when watering is needed. The piezo buzzer sounds during watering to alert the user. 

![image](https://github.com/user-attachments/assets/5221b55c-8280-4b81-a0e2-daa3091239f9)

**_Figure 3: Ultrasonic Sensor Circuit Design_**

**LCD Display Integration:**

Add an LCD screen displays the real-time clock and soil moisture percentage.

![image](https://github.com/user-attachments/assets/41dd840b-e424-466f-9f6b-9a21830703e1)

**_Figure 4: LCD Screen Circuit Design_**

**LCD Display Integration**
To connect the soil moisture readings to the LCD, the signal must be converted from analog to digital across the Arduinos. This is done by filtering the signal through a capacitor and resistor to clean the PWM signal. 

![image](https://github.com/user-attachments/assets/4e8bf86e-3239-4440-9d50-e741bc168066)

**_Figure 6: Multi-Arduino Integration Circuit Design_**

### Part 3: Code
## 1. Red Board Code 

This code includes the soil moisture sensor, ultrasonic sensor, and the indicator LED. Within this code, it is important to define pin definitions and only use one void loop function. This code is outlined below. 

```c++ 
#include <Wire.h>

// === Pin Definitions ===
const int soilSensorPin = A0;
const int pwmOutPin     = 3;    // PWM to green Arduino
const int trigPin       = 7;    // Ultrasonic TRIG
const int echoPin       = 10;    // Ultrasonic ECHO
const int ledPin        = 8;    // LED for low reservoir
const int buzzerPin     = 9;    // Buzzer



// === Thresholds ===

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
const int pumpPin = 10; // relay pin for pump

const int dryThreshold = 450  ; // dry threshold for watering 

const int timedelpump = 8000;
unsigned long timeRun = 0;


// --- Light Schedule (24hr format) ---
const int lightOnHour = 8;
const int lightOffHour = 20;
const unsigned long wateringTime = 2000; //Pump on Time (ms)

String Command;           //the direction that the robot will drive in (this change which direction the two motors spin in)

bool manual = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(pumpPin, OUTPUT);

  // LCD
  lcd.begin(16, 2);
  lcd.print("Starting...");
  delay(1000);
  lcd.clear();

  // pump
  digitalWrite(pumpPin, LOW); // ensure pump is off

  // Relay
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);  // Light OFF at start

  // RTC
  if (!rtc.begin()) {
    lcd.print("RTC not found!");
    Serial.println("RTC not found!");
    while (1);
  }
  timeRun = millis();
  // Set time once (optional, then comment out)
  // rtc.adjust(DateTime(2025, 4, 21, 15, 45, 0));
}

void loop() {
  DateTime now = rtc.now();
  int hour = now.hour();
  int minute = now.minute();
                                                    //if the switch is in the ON position
    if (Serial.available() > 0)                         //if the user has sent a command to the RedBoard
    {
      Command = Serial.readStringUntil(' ');       //read the characters in the command until you reach the first space

      //print the command that was just received in the serial monitor
      Serial.println(Command);
      

      if (Command == "TPON")                         // pump on
      {
        digitalWrite(pumpPin, HIGH);
        delay(wateringTime);
        digitalWrite(pumpPin, LOW);
      }
      
      else if (Command == "TLON")                     // light on
      {
        digitalWrite(relayPin, HIGH);
      }
      else if (Command == "TLOF")                   // light off
      {
        digitalWrite(relayPin, LOW);
      }
      else if (Command == "TLMA")                   // light off
      {
        if (manual == true)
        { 
          manual = false;
          Serial.println("Turned Auto");
        }
        else 
       { 
          manual = true;
          Serial.println("Turned Manual");
        }
      }
    }


  // --- Light Control (Relay) ---
  if ((!manual) && (hour >= lightOnHour) && (hour < lightOffHour))
  {
    digitalWrite(relayPin, HIGH);  // Light ON
    Serial.println(hour);
    Serial.println("💡 Light ON");} 
  else if ((!manual) && (hour <= lightOnHour) && (hour > lightOffHour))
  {
    digitalWrite(relayPin, LOW);   // Light OFF
    Serial.println("🌙 Light OFF");
  }

  // --- Soil Moisture ---
  int analogValue = analogRead(soilInputPin);  // 0-1023
  int moisturePercent = map(analogValue, 0, 1023, 0, 100);  // Calibrated

  Serial.println(analogValue);
  Serial.println(moisturePercent);

   // --- Trigger buzzer and pump if dry AND water available ---
    if (analogValue <= dryThreshold) 
    {
    
      if ((millis() - timeRun) > timedelpump)
      { 
      Serial.print("Now watering ");
      Serial.print(millis());
      Serial.print(" ");
      Serial.print(timeRun);
      digitalWrite(pumpPin, HIGH);
      delay(wateringTime);
      digitalWrite(pumpPin, LOW);
      Serial.println("Pump OFF");
      timeRun = millis();
      }
    } else {
      Serial.println("No watering needed.");
    }

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

## *Part 4 – The Housing*

As this project is comprised of many different working parts, a wooden box was constructed to house the components separately and safely. It consisted of three main compartments: one large middle compartment and two smaller ones on the top and bottom. The upper compartment is intended to house the grow light and associated Arduinos, whereas the lower compartment contains the reservoir, refill light, and pump, and all other associated Arduinos. The larger middle compartment is intended to house the plants safely while preventing any soil from getting into the other components. 

This design also promotes the implementation of the product in its target audience: busy college students. With one simple housing that contains all components, consumers needn’t worry about having difficulty moving things around – just empty the reservoir and it’s ready to move!

To ensure replicative design via future engineers, SOLIDWORKS 2024 was used to model the housing and create clear and concise engineering drawings before physically building the model. The panels were modeled individually, then laser cut before being superglued together. Support brackets were also created and fastened to the bottom corners of the housing to ensure stability while bearing weight. One drawing for the brackets and each wooden panel, as well as an isometric drawing of the full model are included as follows:
(insert images of drawings) 


## *Part 5 – The Mobile App*
To promote seamless user interfacing, a mobile app was created to bring the different components together in one place. This is the master control for our watering system – here, the user can control everything that the system does. This part delves into the details of how the team designed and implemented each individual section. 

### 1. Automatic Grow Light Timer (Custom Bounds) 
This subsystem automates whether the grow light is on or off depending on user-chosen bounds. A timer was created with the ability to customize the start and end of each “on” interval by clicking a drop-down menu. This timer allows the user to customize their watering schedule based on the plant species or how often they’d like to water it. The code implemented to create this automatic grow light is included as follows:

### 2. Reservoir Water Level Alerts
To ensure the system does not run out of water, an alert feature was created to notify the user when the water level reaches below a certain threshold. The ultrasonic sensor takes the readings and communicates them to MIT app inventor via a Flask server, which is described further in the integration with Arduinos subsystem. When triggered, it will send an alert to the user, instructing them to refill the reservoir. The code used to create the reservoir water level alert system is shown as follows:

### 3. Plant Health Tracking 
Another deliverable the team aimed to satisfy is the plant health tracking. This subsystem keeps a running count of how often the grow light has been turned on and the plant has been watered. It then uses this raw data and displays it on a separate screen that the user can refer to at any time. The code used to produce this is included as follows:

### 4. Nutrient Intake Guide  
When dealing with different types of plants, it’s oftentimes hard to keep track of what each individual plant may need. The nutrient intake guide is a basic outline for lighting and watering practices for a couple of common plants for the user to refer to at any time. For future design, this can be expanded upon to allow for customizable inputs (i.e., Different kinds of houseplants instead of just an umbrella term for houseplants, more fruits, and vegetables, etc.). The code used to implement this is as follows:

### 5. Integration with Arduinos 
Arduino IDE cannot directly communicate with MIT app inventor, which requires a creative design solution to get around this hurdle. There are two computers running two different .ino files, but each computer will run a small python Flask server that listens to MIT app inventor web requests and acts accordingly. This allows the MIT app inventor to effectively communicate with both computers, allowing seamless user experience. The code and steps required to set up these Flask servers are included as follows: 

## Testing Description

We thoroughly tested each component individually and then as a complete system. The testing setup included:

*	Soil Sensor Calibration:
Using a Digital Multimeter to measure voltage outputs across the sensor to test connection. 
*	RTC set up and testing:
Verified and adjusted time using the RTC library from Arduino IDE. 
*	Ultrasonic Sensor testing:
Confirmed distance measurement accuracy for reservoir monitoring.
*	Pump Activation test:
Confirmed relay control and correct moisture threshold activation. 
*	Grow Light Testing:
Verified that the relay correctly switched the light on and off according to the RTC settings. 
*	RTC Adjustments:
Fine-tuned timing to align lighting schedule and moisture readings 

Equipment used:

*	Digital Multimeter (voltage output reading)
*	Arduino IDE Serial Monitor for sensor output confirmation

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

