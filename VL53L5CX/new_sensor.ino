#include <Wire.h>
#include <vl53l5cx_class.h>
#include <WiFi.h>
#include <esp_now.h>

// Define ESP32-C3 default hardware I2C Pins
#define I2C_SDA 4
#define I2C_SCL 5
#define DEV_I2C Wire

VL53L5CX sensor(&DEV_I2C, -1, -1);

/*
SparkFun_VL53L5CX myImager;
VL53L5CX_ResultsData measurementData;

int imageResolution = 0; // Totals zones (16 for 4x4, 64 for 8x8)
int imageWidth = 0;      // Rows/Columns boundary
*/
void setup() {
  Serial.begin(115200);
  delay(2000); 
  Serial.println("ESP32-C3 + VL53L5CX ToF Test");

  // Force I2C initialization with the specific ESP32-C3 pins
  DEV_I2C.begin(I2C_SDA, I2C_SCL);
  DEV_I2C.setClock(400000); // VL53L5CX supports up to 400kHz standard

  Serial.println("Uploading firmware blob to VL53L5CX. Please wait up to 10s...");


  if (sensor.begin() == false) {
    Serial.println("Sensor failed to initialize! Check wiring/pullups.");
    while (1);
  }
  else{

    sensor.begin();
    Serial.println("Sensor initialized successfully!");

  }

  // 4x4 resolution is recommended for stable ESP32-C3 RAM usage
  sensor.setResolution(4 * 4); 
  imageResolution = sensor.getResolution(); 
  imageWidth = sqrt(imageResolution);




  sensor.init_sensor();
  sensor.vl53l5cx_start_ranging();

  Serial.println("Ranging started successfully!");
}

void loop() {
  // Check if the sensor has finished a scanning cycle
  VL53L5CX_ResultsData Results;
  uint8_t NewDataReady = 0;
  char report[64];
  uint8_t status;

  do {
      status = sensor.vl53l5cx_check_data_ready(&NewDataReady);
    } while (!NewDataReady);
  
  if ((!status) && (NewDataReady != 0)) {
      status = sensor.vl53l5cx_get_ranging_data(&Results);

      /* As the sensor is set in 4x4 mode by default, we have a total
       * of 16 zones to print.
       */

      snprintf(report, sizeof(report), "Print data no : %3u\r\n", sensor.get_stream_count());
      SerialPort.print(report);
      for (int i = 0; i < 16; i++) {
        snprintf(report, sizeof(report), "Zone : %3d, Status : %3u, Distance : %4d mm\r\n",
                 i,
                 Results.target_status[VL53L5CX_NB_TARGET_PER_ZONE * i],
                 Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE * i]);
        SerialPort.print(report);
      }
      SerialPort.println("");
    }

/*
  if (sensor.isDataReady() == true) {
    if (sensor.getRangingData(&measurementData)) {
      
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
  */
  delay(50); // Small cycle delay to free up CPU
}
