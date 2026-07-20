#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <SparkFun_VL53L5CX_Library.h> //http://librarymanager/All#SparkFun_VL53L5CX
#include <list>
#include <unordered_set>


#define I2C_SDA D4
#define I2C_SCL D5

void setup() 
{
    Serial.println(sensor.getAddress());

    Serial.println("Initializing sensor board. This can take up to 10s. Please wait.");
    if (sensor.begin() == false)
    {
    Serial.println(F("Sensor not found - check your wiring. Freezing"));
    while (1) ;
    } else{
    Serial.println("Sensor has successfully begun.");   
    }

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
Serial.println("hi");