

// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <VL53L1X.h>
#define SDA_PIN 8
#define SCL_PIN 9




// Variables for test data
int int_value;
float float_value;
bool bool_value = true;
VL53L1X sensor;




// MAC Address of responder - edit as required 94:A9:90:6A:7A:58.
// esp #1:  94:A9:90:67:22:EC
// esp #2:  94:A9:90:67:03:F8


//uint8_t broadcastAddress[] = {0x94, 0xA9, 0x90, 0x6A, 0x7A, 0x58};
uint8_t broadcastAddress[] = {0x94, 0xA9, 0x90, 0x67, 0x22, 0xEC};  // rcvr #1
//uint8_t broadcastAddress[] = {0x94, 0xA9, 0x90, 0x67, 0x03, 0xF8};  // rcvr #2


















// Create a structured object












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
 Wire.setClock(400000); // use 400 kHz I2C
 sensor.setDistanceMode(VL53L1X::Long);




// Initilize ESP-NOW
 if (esp_now_init() != ESP_OK) {
   Serial.println("Error initializing ESP-NOW");
   return;
 }




 // test sensor detection
 sensor.setTimeout(500);
 if (!sensor.init()){
   Serial.println("Failed to detect and initialize sensor!");
   while (1);
 }








// Register the send callback
 esp_now_register_send_cb(OnDataSent);
 // Register peer
 memcpy(peerInfo.peer_addr, broadcastAddress, 6);
 peerInfo.channel = 0;
 peerInfo.encrypt = false;
 // Add peer     
 if (esp_now_add_peer(&peerInfo) != ESP_OK){
   Serial.println("Failed to add peer");
   return;
 }
//set up sensor
 sensor.setDistanceMode(VL53L1X::Long);
 sensor.setMeasurementTimingBudget(50000);
 sensor.startContinuous(50);




 sensor.setROISize(4, 4);    // very narrow (~15°)
 sensor.setROICenter(199);
}












void loop() {




 //distance
 float distance;
 distance = sensor.read();








 // Send message via ESP-NOW
 esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &distance, sizeof(distance));




 Serial.print(distance);
 if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }




 Serial.println();
 if (result == ESP_OK) {
   Serial.println("Sending confirmed");
 }
 else {
   Serial.println("Sending error");
 }
 delay(200);
}