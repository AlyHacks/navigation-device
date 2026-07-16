#include <esp_now.h>
#include <WiFi.h>


#include <Wire.h>

const int buzzerPin = D7; // GPIO pin connected to the buzzer



void setup() {
    Wire.begin();



    // Set up Serial Monitor
    Serial.begin(115200);
    // Set ESP32 as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    // Initilize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }


    pinMode(buzzerPin, OUTPUT); // Set the pin as an output
}

void loop() {
    Serial.println("hi");
    digitalWrite(buzzerPin, HIGH); // Turn the buzzer on
    delay(1000);                // Wait for 1 second
    digitalWrite(buzzerPin, LOW);  // Turn the buzzer off
    delay(1000);                // Wait for 1 second


}