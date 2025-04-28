# Flower Power Plant Staycation

## Summary

The **Flower Power Plant Staycation** was thoughtfully designed with busy college students in mind — especially those who love their plants but may not always have the time to properly care for them. This innovative system features a fully automated watering and lighting setup that users can customize based on their plant’s specific needs. A SparkFun Soil Moisture Sensor continuously monitors soil moisture, triggering watering only when necessary to conserve water and prevent overwatering. An ultrasonic sensor monitors the water reservoir level and activates an LED indicator when a refill is needed. By automating essential plant care tasks, the Flower Power Plant Staycation helps students maintain healthy, thriving plants with minimal daily effort — even during exams or weekends away.

---

## Design Description

### Materials and Planning
We began by outlining the materials needed for the project. Key components were carefully selected based on their performance and compatibility:
- UV Grow Light to promote optimal plant growth
- Water pump motor capable of extended use
- Circuit parts, including an RTC (Real-Time Clock) module and a relay

**_Figure 1: Bill of Materials_**

_(Insert picture here)_

---

### Part 1: Automated Watering System

**Motor and Soil Sensor Control:**
- The motor is connected to a relay, which switches it on and off based on soil moisture readings.
- A SparkFun Soil Moisture Sensor detects when watering is needed.
- A piezo buzzer sounds during watering to alert the user.

**_Figure 2: Motor and Soil Sensor Circuit Design_**

_(Insert picture here)_

**Reservoir Monitoring:**
- An ultrasonic sensor detects the water level in the reservoir.
- If the water level drops too low, an LED light illuminates to signal a refill is needed.

**_Figure 3: Ultrasonic Sensor Circuit Design_**

_(Insert picture here)_

**LCD Display Integration:**
- An LCD screen displays the real-time clock and soil moisture percentage.

_(Insert LCD picture here)_

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

# 🌿 Flower Power Project | Designed by [Your Name/Team Name]
