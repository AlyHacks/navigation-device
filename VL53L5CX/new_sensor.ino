// claire learns how to use github
// hopefully claire didn't break anything

#include <Wire.h>
#include <vl53l5cx_class.h>
#include <WiFi.h>
#include <esp_now.h>

// Define ESP32-C3 default hardware I2C Pins
#define I2C_SDA D8
#define I2C_SCL D9
#define DEV_I2C Wire
#define LPN D6
#define RST D5
#define VL53L5CX_RANGING_MODE_AUTONOMOUS  ((uint8_t) 3U)
//#define VL53L5CX_RESOLUTION_4X4 ((uint8_t)16U)

VL53L5CX sensor(&DEV_I2C, LPN, RST);

/*
SparkFun_VL53L5CX myImager;
VL53L5CX_ResultsData measurementData;
*/


//int imageResolution = 0; // Totals zones (16 for 4x4, 64 for 8x8)
//int imageWidth = 0;      // Rows/Columns boundary

void setup() {

  char report[64];
  uint8_t status;

  Serial.begin(115200);
  delay(2000); 
  Serial.println("ESP32-C3 + VL53L5CX ToF Test");

  // Force I2C initialization with the specific ESP32-C3 pins
  DEV_I2C.setBufferSize(256);
  DEV_I2C.begin();
  DEV_I2C.setClock(100000); // VL53L5CX supports up to 400kHz standard

  Serial.println("Uploading firmware blob to VL53L5CX. Please wait up to 10s...");

  //sensor.vl53l5cx_set_resolution(VL53L5CX_RESOLUTION_4X4);

  


/*
  if (sensor.begin() == false) {
    Serial.println("Sensor failed to initialize! Check wiring/pullups.");
    while (1);
  }
  else{

    sensor.begin();
    Serial.println("Sensor initialized successfully!");

  }

*/

  // 4x4 resolution is recommended for stable ESP32-C3 RAM usage
  /*
  sensor.setResolution(4 * 4); 
  imageResolution = sensor.getResolution(); 
  imageWidth = sqrt(imageResolution);
*/

  //uint8_t status;
  status = sensor.begin();
  Serial.print("begin() status: ");
  Serial.println(status);

  delay(100);

  status = sensor.init_sensor();
  Serial.print("init_sensor() status: ");
  Serial.println(status);
  if (status != 0) {
    Serial.println("init_sensor failed, halting.");
    while(1);
  
  //DEV_I2C.setClock(400000);
  }
  status = sensor.vl53l5cx_set_ranging_mode(VL53L5CX_RANGING_MODE_AUTONOMOUS);
  if (status) {
    snprintf(report, sizeof(report), "vl53l5cx_set_ranging_mode failed, status %u\r\n", status);
    Serial.print(report);
  } else{
    
    Serial.println("Ranging mode set to autonomous successfully!");

  }

  /* Using autonomous mode, the integration time can be updated (not possible
   * using continuous) */
  status = sensor.vl53l5cx_set_integration_time_ms(20);

  if (status) {
    snprintf(report, sizeof(report), "vl53l5cx_set_integration_time_ms failed, status %u\r\n", status);
    Serial.print(report);
  }


  if(sensor.vl53l5cx_start_ranging() != 0) {
    Serial.println("Error during ranging");
  } else {
    Serial.println("Ranging started successfully!");
  }
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

      Serial.println("Connected");

      /* As the sensor is set in 4x4 mode by default, we have a total
       * of 16 zones to print.
       */

      /*

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

    */

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
  }
  delay(50); // Small cycle delay to free up CPU
}
