#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include "esp_wifi.h"

const int buzzerPin = D8; // GPIO pin connected to the buzzer
int received_distance = 1;
int ontime = 200;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    unsigned char a[32];
    int d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

int distance;

// callback function that will be executed when data is received
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println("Distance: ");
  distance = myData.d;
  Serial.print(distance);
}
 
void setup() {

  pinMode(buzzerPin, OUTPUT);
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(1000);
  Serial.print("hello from buzzer esp32");
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  esp_wifi_set_ps(WIFI_PS_NONE);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  Wire.begin();
  Wire.setClock(400000);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else {
    Serial.println("success initializing esp-now");
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  if (distance < 100){
    digitalWrite(buzzerPin, HIGH);
    delay(ontime);
    digitalWrite(buzzerPin, LOW); 
    delay(distance);
  } else if(distance < 300){
    digitalWrite(buzzerPin, HIGH);
    delay(ontime);
    digitalWrite(buzzerPin, LOW);
    delay(distance);
  } else if (distance < 500){
    digitalWrite(buzzerPin, HIGH);
    delay(ontime);
    digitalWrite(buzzerPin, LOW);
    delay(distance);
  } else {
    digitalWrite(buzzerPin, LOW);
    Serial.print("over 500");
  }
delay(150);
}