# ESP-32 C-3 Navigation Device

## Introduction
This navigation device uses an Arduino Time of Flight infrared sensor to detect nearby obstacles/objects in its field of view. If there are objects within 1 foot (30 cm) from the user, the LED will turn on for feedback.

## Components

1. Arduino Tof Sensor
2. ESP-32 C-3 (2x)
3. Lithium LiPo rechargeable batteries 
4. Resistor
5. Wires
6. Solder board
7. Switches
8. Haptic Buzzer

## How it works
1. The ToF Sensor sends out an infrared wave.
2. Measures the amount of time it took for the wave to bounce back (off of an object).
3. Calculates the distance the object is from the sensor (using the amount of time it took for signal to bounce back).
4. If the object is closer than 30 cm, the haptic buzzer turns on.
5. If the object is less than 10 cm, the haptic buzzer buzzes with increasing frequency as object moves closer (suggesting urgency to the user).
6. If the object is farther than 30 cm, the haptic buzzer is off.

## Conclusion
This device uses haptic feedback for the user to help them better navigate their environment, by alerting them when obstacles are near. The aim of this project is to help familiarize the user with the environment, fostering independence when navigating their surroundings.
