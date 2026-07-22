#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <SparkFun_VL53L5CX_Library.h> //http://librarymanager/All#SparkFun_VL53L5CX
#include <list>
#include <unordered_set>


#define I2C_SDA D4
#define I2C_SCL D5


const int buzzerPin = D7; // GPIO pin connected to the buzzer

/*
//later, find a simpler way to calculate averages of quadrants
//also double check these numbers are even correct
std::unordered_set<int> left = {1, 2, 3, 4, 9, 10, 11, 12, 17, 18, 19, 20, 25, 26, 27, 28, 33, 34, 35, 36, 41, 42, 43, 44, 49, 50, 51, 52, 57, 58, 59, 60}; 
std::unordered_set<int> right = {5, 6, 7, 8, 13, 14, 15, 16, 21, 22, 23, 24, 29, 30, 31, 32, 37, 38, 39, 40, 45, 46, 47, 48, 53, 54, 55, 56, 61, 62, 63, 64};

//initiating variables to use later for measuring average distance of the 4 quadrants
int average_dist_l;
int average_dist_r;
*/





void setup() {
  pinMode(buzzerPin, OUTPUT);
  // Set up Serial Monitor
  Serial.begin(115200);
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
  digitalWrite(ledPin, HIGH);
  delay(ontime);
  digitalWrite(ledPin,LOW);


  void OnDataRecv(const esp_now_recv_info * mac, const uint8_t *incomingData, int len) {
    memcpy(&received_distance, incomingData, sizeof(received_distance));
    Serial.print("Data received: ");
    Serial.println(len);
    Serial.print("Distance:");
    Serial.println(received_distance);
    Serial.println();
}

}


void loop()
{


    if (received_distance < 200){
            digitalWrite(buzzerPin, HIGH); // Turn the buzzer on
            delay(100);                // Wait for 1 second
            digitalWrite(buzzerPin, LOW);  // Turn the buzzer off
            delay(100);                // Wait for 1 second
    }else if (received_distance < 500){
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

  delay(5); //Small delay between polling
}