#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <SparkFun_VL53L5CX_Library.h> //http://librarymanager/All#SparkFun_VL53L5CX
#include <iostream>
#include <list>
#include <unordered_set>
#define I2C_SDA D4
#define I2C_SCL D5

SparkFun_VL53L5CX sensor;
VL53L5CX_ResultsData results; // Result data class structure, 1356 byes of RAM

int imageResolution = 0; //Used to pretty print output
int imageWidth = 0; //Used to pretty print output

uint8_t broadcastAddress[] = {0xAC, 0x27, 0x6E, 0x73, 0xA3, 0xA8};  // rcvr address so esp of the buzzer

// Peer info
esp_now_peer_info_t peerInfo;

String success;


// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if(status == 0) {
    success = "Delivery Success";
  } else {
    success = "Delivery Fail";
  }
}


void setup() {
  Serial.begin(115200);
  delay(1000);
  Wifi.mode(WIFI_STA);
  Wire.begin();
  Wire.setClock(400000);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Serial.print("hi");
  Serial.println(sensor.getAddress());



  Serial.println("Initializing sensor board. This can take up to 10s. Please wait.");
  if (sensor.begin() == false) {
    Serial.println(F("Sensor not found - check your wiring. Freezing"));
    while (1);
  } else {
    Serial.println("Sensor has successfully begun.");   
  }

  sensor.setResolution(8*8); //Enable all 64 pads
    
  imageResolution = sensor.getResolution(); //Query sensor for current resolution - either 4x4 or 8x8
  imageWidth = int(sqrt(imageResolution)); //Calculate printing width
   
    /*
    if (sensor.startRanging() == false){
      Serial.println(F("Failed to start ranging. Freezing"));
      while (1) ;
    } else{
      
      Serial.println("Ranging has successfully started.");
    }
    */

  sensor.startRanging();
  Serial.println("ranging started");

}

void loop() {

  int distance;

  

  if (sensor.isDataReady() == true) {
    if (sensor.getRangingData(&results)) { //Read distance data into array 
        //The ST library returns the data transposed from zone mapping shown in datasheet
        //Pretty-print data with increasing y, decreasing x to reflect reality
        for (int y = 0 ; y <= imageWidth * (imageWidth - 1) ; y += imageWidth) {
          for (int x = imageWidth - 1 ; x >= 0 ; x--) {
            Serial.print(" Distance results:");
            distance = results.distance_mm[x + y];
          }
          
          Serial.println();
        }      
    } 
  }

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &distance, sizeof(distance));

  if(result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending data")
  }

  delay(5);
}