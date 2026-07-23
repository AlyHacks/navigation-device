#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <SparkFun_VL53L5CX_Library.h> //http://librarymanager/All#SparkFun_VL53L5CX
#include <list>
#include <unordered_set>

// mac address of buzzer: AC:27:6E:7E:A3:A8


#define I2C_SDA D4
#define I2C_SCL D5

int ontime = 200;
const int buzzerPin = D7; // GPIO pin connected to the buzzer
int received_distance = 1;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&received_distance, incomingData, sizeof(received_distance));
    Serial.print("Data received: ");
    Serial.println(len);
    Serial.print("Distance:");
    Serial.println(received_distance);
    Serial.println();
  }


void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
  delay(1000);
  pinMode(buzzerPin, OUTPUT);
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  delay(1000);
  Serial.println("SparkFun VL53L5CX Imager Example");

  //Wire.begin(6,7); //This resets to 100kHz I2C
  Wire.begin();
  Wire.setClock(400000); //Sensor has max I2C freq of 400kHz 


  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);


  // single buzz on startup
  digitalWrite(buzzerPin, HIGH);
  delay(ontime);
  digitalWrite(buzzerPin, LOW);


}


void loop() {

  Serial.print(received_distance);

/*
    if (received_distance < 200) {
            Serial.println("distance is < 200");
            digitalWrite(buzzerPin, HIGH); // Turn the buzzer on
            delay(100);                // Wait for 1 second
            digitalWrite(buzzerPin, LOW);  // Turn the buzzer off
            delay(100);                // Wait for 1 second
    }else if (received_distance < 500){
            Serial.println("distance is < 500");
            digitalWrite(buzzerPin, HIGH); // Turn the buzzer on
            delay(250);                // Wait for 1 second
            digitalWrite(buzzerPin, LOW);  // Turn the buzzer off
            delay(250);                // Wait for 1 second
    } else {
            digitalWrite(buzzerPin, LOW);
            delay(2000); // Turn the buzzer off
            Serial.println("distance is > 500");
        }
    received_distance = 0;

    */

  delay(5); //Small delay between polling
}