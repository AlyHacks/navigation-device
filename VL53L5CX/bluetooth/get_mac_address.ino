#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  
  // Set the Wi-Fi mode to Station or AP
  WiFi.mode(WIFI_STA);
   
  // Get and print the MAC address
  String mac = WiFi.macAddress();
  Serial.print("ESP32-C3 MAC Address: ");
  Serial.println(mac);
}

void loop() {
}
