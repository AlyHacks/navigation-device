#include <Wire.h>
#include <SparkFun_VL53L5CX_Library.h>
#include <WiFi.h>
#include <esp_now.h>

// Define ESP32-C3 default hardware I2C Pins
#define I2C_SDA 8
#define I2C_SCL 9

SparkFun_VL53L5CX myImager;
VL53L5CX_ResultsData measurementData;

int imageResolution = 0; // Totals zones (16 for 4x4, 64 for 8x8)
int imageWidth = 0;      // Rows/Columns boundary

void setup() {
  Serial.begin(115200);
  delay(2000); 
  Serial.println("ESP32-C3 + VL53L5CX ToF Test");

  // Force I2C initialization with the specific ESP32-C3 pins
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000); // VL53L5CX supports up to 400kHz standard

  Serial.println("Uploading firmware blob to VL53L5CX. Please wait up to 10s...");
  
  if (myImager.begin() == false) {
    Serial.println("Sensor failed to initialize! Check wiring/pullups.");
    while (1);
  }

  // 4x4 resolution is recommended for stable ESP32-C3 RAM usage
  myImager.setResolution(4 * 4); 
  imageResolution = myImager.getResolution(); 
  imageWidth = sqrt(imageResolution);

  myImager.startRanging();
  Serial.println("Ranging started successfully!");
}

void loop() {
  // Check if the sensor has finished a scanning cycle
  if (myImager.isDataReady() == true) {
    if (myImager.getRangingData(&measurementData)) {
      
      Serial.println("\n--- Distance Grid (mm) ---");
      
      // Iterate through the matrix grid to print layout
      for (int i = 0; i < imageResolution; i++) {
        Serial.print(measurementData.distance_mm[i]);
        Serial.print("\t");
        
        // Break line at the end of the matrix width
        if ((i + 1) % imageWidth == 0) {
          Serial.println();
        }
      }
    }
  }
  delay(50); // Small cycle delay to free up CPU
}
