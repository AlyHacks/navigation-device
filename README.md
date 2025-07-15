# Ultrasonic Navigation Device

## Introduction
This Arduino device uses HC-SR04 as an ultrasonic sensor to detect nearby obstacles/objects in the way. If there is an object within 1 foot (30 cm) from the user, the LED will turn on for feedback.

## Components

1. Arduino Nano
2. HC-SR04 Ultrasonic Sensor
3. LED
4. Resistor
5. Jumper Wires
6. Transistor
7. Battery/Power supply
8. Breadboard

## How it works
1. The Ultrasonic Sensor sends out a sound wave.
2. Measures amount of time it took for sound wave to bounce back (off of an object).
3. Calculates the distance the object is from the sensor (using the amount of time it took for signal to bounce back).
4. If the object is closer than 30 cm, LED turns on.
5. If the object is less than 10 cm, LED blinks.
6. If the object is farther than 30 cm, LED is off.

## Conclusion
This device uses LED (haptic) feedback for the user to help them better navigate their environment, by alerting them when obstacles are near. The aim of this project is to help familiarize the user with the environment, fostering independence when navigating their surroundings.
