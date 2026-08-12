#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include "esp_wifi.h"
#include <SparkFun_VL53L5CX_Library.h>
#include <iostream>
#include <algorithm>

#define I2C_SDA D4
#define I2C_SCL D5

SparkFun_VL53L5CX sensor;
VL53L5CX_ResultsData results;

int imageResolution = 0; 
int imageWidth = 0;

//led MAC address: 94:A9:90:77:70:D0
uint8_t broadcastAddress[] = {0x94, 0xA9, 0x90, 0x77, 0x70, 0xD0};

// Structure example to send data
// Must match the receiver structure

// Create a struct_message called myData
//struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  delay(1000);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  esp_wifi_set_ps(WIFI_PS_NONE);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 1;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // Serial.println(sensor.getAddress());

  Serial.println("Initializing sensor board. This can take up to 10s. Please wait.");
  if (sensor.begin() == false)
  {
  Serial.println(F("Sensor not found - check your wiring. Freezing"));
  while (1) ;
  } else {
  Serial.println("Sensor has successfully begun.");
  }

  sensor.setResolution(8*8); //Enable all 64 pads
  
  imageResolution = sensor.getResolution(); //Query sensor for current resolution - either 4x4 or 8x8
  imageWidth = int(sqrt(imageResolution)); //Calculate printing width

  sensor.startRanging();
  Serial.print("sensor has started ranging.");


}
 
void loop() {

  int minimum = 1000000; //fake value to be replaced in the minimum logic
 
  //Poll sensor for new data
  if (sensor.isDataReady() == true) {
    if (sensor.getRangingData(&results)) { //Read distance data into array
        //The ST library returns the data transposed from zone mapping shown in datasheet
        //Pretty-print data with increasing y, decreasing x to reflect reality

      for (int y = 0 ; y <= imageWidth * (imageWidth - 1) ; y += imageWidth) {
        for (int x = imageWidth - 1 ; x >= 0 ; x--) {
          int distance = results.distance_mm[x+y];

          int status = results.target_status[x+y];
          if (status == 5 && distance < minimum) { // status 5 means valid target
            minimum = distance;
          }
        }
      }

    }

        // Send message via ESP-NOW
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &minimum, sizeof(minimum));
        
        if (result == ESP_OK) {
          Serial.println("Sent with success");
        }
        else {
          Serial.println("Error sending the data");
        }

      } else {
        Serial.print("error");
      }  

    delay(100);
}
    
  


