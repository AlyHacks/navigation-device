// Include Libraries
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <vl53l4cx_class.h>

#define SDA_PIN 3
#define SCL_PIN 2

#define DEV_I2c Wire
#define SerialPort Serial

VL53L4CX sensor(&Wire, -1); //-1 means XSHUT isn't connected


// MAC Address of responder - edit as required 94:A9:90:6A:7A:58.
// esp #1:  94:A9:90:67:22:EC
// esp #2:  94:A9:90:67:03:F8


//uint8_t broadcastAddress[] = {0x94, 0xA9, 0x90, 0x6A, 0x7A, 0x58};
//uint8_t broadcastAddress[] = {0x94, 0xA9, 0x90, 0x67, 0x22, 0xEC};  // rcvr #1
//uint8_t broadcastAddress[] = {0x94, 0xA9, 0x90, 0x67, 0x03, 0xF8};  // rcvr #2

// Peer info
esp_now_peer_info_t peerInfo;


// Callback function called when data is sent
void OnDataSent(const wifi_tx_info_t *mac_addr, esp_now_send_status_t status) {
Serial.print("\r\nLast Packet Send Status:\t");
Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void setup() {
// Set up Serial Monitor
 Serial.begin(115200);
 // Set ESP32 as a Wi-Fi Station
 WiFi.mode(WIFI_STA);
 Wire.begin(SDA_PIN, SCL_PIN);

 sensor.begin();
 //sensor.VL53L4CX_Off();
 //sensor.InitSensor(0x12)
 sensor.VL53L4CX_StartMeasurement();

// Initilize ESP-NOW
 if (esp_now_init() != ESP_OK) {
   Serial.println("Error initializing ESP-NOW");
   return;
 }

}

void loop() {
  uint8_t NewDataReady = 0;
  VL53L4CX_MultiRangingData_t MultiRangingData;

  sensor.VL53L4CX_GetMeasurementDataReady(&NewDataReady);

  if (NewDataReady) {
    sensor.VL53L4CX_GetMultiRangingData(&MultiRangingData);

    Serial.println(MultiRangingData.RangeData[0].RangeMilliMeter);

    sensor.VL53L4CX_ClearInterruptAndStartMeasurement();
  }

  delay(200);

}



 