#include <Arduino.h>

void setup() {
  Serial.begin(230400);   // use the DEFAULT rate for this test
  delay(1000);
  Serial.println("Boot OK");
}

void loop() {
  Serial.println("alive");
  delay(1000);
}

/*

// Include Libraries
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <vl53l4cx_class.h>

#define SDA_PIN D4
#define SCL_PIN D5

#define DEV_I2C Wire
//#define SerialPort Serial

VL53L4CX sensor(&DEV_I2C, -1); //-1 means XSHUT isn't connected


// MAC Address of responder - edit as required 94:A9:90:6A:7A:58.
// esp #1:  94:A9:90:67:22:EC
// esp #2:  94:A9:90:67:03:F8

// esp#3 THUS ONE: 94:a9:90:77:7b:c8
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
  Serial.begin(230400);
  Serial.println("Starting...");
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  DEV_I2C.begin(SDA_PIN, SCL_PIN);

  //int status;
  //int status_1;
  //int status_2;

  int status = sensor.begin();
  Serial.print("begin(): "); Serial.println(status);

  if (status != 0) {
    Serial.println("sensor.begin() failed");
  }

  sensor.VL53L4CX_Off();

  //0 = success!

  int status_1 = sensor.InitSensor(0x29);
  Serial.print("InitSensor(): "); Serial.println(status_1);
  if (status_1 != 0) {
    Serial.println("InitSensor failed");
  }

  /*
  status_2 = sensor.VL53L4CX_StartMeasurement();
  Serial.println(status_2);
  if (status_2 != 0) {
    Serial.println("StartMeasurement failed");
  }
  else{
    sensor.VL53L4CX_StartMeasurement();
  }
  
  sensor.VL53L4CX_SetMeasurementTimingBudgetMicroSeconds(100000);

  int status_2 = sensor.VL53L4CX_StartMeasurement();
  Serial.print("StartMeasurement(): "); Serial.println(status_2);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
}

void loop() {



  Serial.println("hi");

  delay(2000);

  //DEV_I2C.requestFrom(SDA_PIN, SCL_PIN);
  uint8_t NewDataReady = 0;
  VL53L4CX_MultiRangingData_t MultiRangingData;
  VL53L4CX_MultiRangingData_t *pMultiRangingData = &MultiRangingData;

  sensor.VL53L4CX_GetMeasurementDataReady(&NewDataReady);

  if (NewDataReady) {
    sensor.VL53L4CX_GetMultiRangingData(&MultiRangingData);

    if (MultiRangingData.NumberOfObjectsFound > 0) {
      Serial.println(MultiRangingData.RangeData[0].RangeMilliMeter);
    }

    sensor.VL53L4CX_ClearInterruptAndStartMeasurement();


    
  }

  delay(200);
}

*/