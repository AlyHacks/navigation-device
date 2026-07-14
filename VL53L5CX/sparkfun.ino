#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <SparkFun_VL53L5CX_Library.h> //http://librarymanager/All#SparkFun_VL53L5CX

SparkFun_VL53L5CX sensor;
VL53L5CX_ResultsData results; // Result data class structure, 1356 byes of RAM

int imageResolution = 0; //Used to pretty print output
int imageWidth = 0; //Used to pretty print output

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("SparkFun VL53L5CX Imager Example");

  Wire.begin(); //This resets to 100kHz I2C
  Wire.setClock(400000); //Sensor has max I2C freq of 400kHz 
  
  Serial.println("Initializing sensor board. This can take up to 10s. Please wait.");
  if (sensor.begin() == false)
  {
    Serial.println(F("Sensor not found - check your wiring. Freezing"));
    while (1) ;
  } else{
    Serial.println("Sensor has successfully begun.");
  }


  if (sensor.init_sensor() == false)
  {
    Serial.println(F("Sensor initialization failed. Freezing"));
    while (1) ;
  } else{
  Serial.println("Sensor has successfully initialized.");
  }

  sensor.setResolution(8*8); //Enable all 64 pads
  
  imageResolution = sensor.getResolution(); //Query sensor for current resolution - either 4x4 or 8x8
  imageWidth = sqrt(imageResolution); //Calculate printing width
  
  if (sensor.startRanging() == false){
    Serial.println(F("Failed to start ranging. Freezing"));
    while (1) ;
  } else{
    Serial.println("Ranging started successfully!");
  }

}

void loop()
{
  //Poll sensor for new data
  if (sensor.isDataReady() == true)
  {
    if (sensor.getRangingData(&results)) //Read distance data into array
    {
      //The ST library returns the data transposed from zone mapping shown in datasheet
      //Pretty-print data with increasing y, decreasing x to reflect reality
      for (int y = 0 ; y <= imageWidth * (imageWidth - 1) ; y += imageWidth)
      {
        for (int x = imageWidth - 1 ; x >= 0 ; x--)
        {
          Serial.print("Distance results:");
          Serial.print(results.distance_mm[x + y]);
        }
        Serial.println();
      }
      Serial.println();
    }
  }

  delay(5); //Small delay between polling
}