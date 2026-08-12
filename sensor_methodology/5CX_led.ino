#include <esp_now.h>
#include <WiFi.h>
#include "esp_wifi.h"

const int LED_PIN = D10;

int received_minimum;
int ontime = 200;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&received_minimum, incomingData, sizeof(received_minimum));
  //Serial.println("Distance: ");
  //Serial.print(received_minimum);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  delay(1000);
  Serial.print("hello from led esp32");

  WiFi.mode(WIFI_STA);
  esp_wifi_set_ps(WIFI_PS_NONE);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else {
    Serial.println("success initializing esp-now");
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  Serial.println(received_minimum);
  if (received_minimum <= 500){
    digitalWrite(LED_PIN, HIGH);
    delay(ontime);
    digitalWrite(LED_PIN, LOW);
    delay(received_minimum);
    //delay(100);
    //digitalWrite(LED_PIN, LOW);
    //delay(100);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

delay(100);

}