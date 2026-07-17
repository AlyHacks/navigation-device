#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <SparkFun_VL53L5CX_Library.h> //http://librarymanager/All#SparkFun_VL53L5CX
#include <Wire.h>
#include <esp_now.h>
#include <WiFi.h>
#include <list>


#define I2C_SDA D4
#define I2C_SCL D5


const int buzzerPin = D7; // GPIO pin connected to the buzzer



SparkFun_VL53L5CX sensor;
VL53L5CX_ResultsData results; // Result data class structure, 1356 byes of RAM

int imageResolution = 0; //Used to pretty print output
int imageWidth = 0; //Used to pretty print output

void setup()
{
    pinMode(buzzerPin, OUTPUT);
    Serial.begin(115200);
    delay(1000);
    Serial.println("SparkFun VL53L5CX Imager Example");

  //Wire.begin(6,7); //This resets to 100kHz I2C
    Wire.begin();
    Wire.setClock(400000); //Sensor has max I2C freq of 400kHz 

    Serial.println(sensor.getAddress());

    Serial.println("Initializing sensor board. This can take up to 10s. Please wait.");
    if (sensor.begin() == false)
    {
    Serial.println(F("Sensor not found - check your wiring. Freezing"));
    while (1) ;
    } else{
    Serial.println("Sensor has successfully begun.");
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

}

void loop()
{
    std::list<int> distance;
    int average_dist = 0;
 

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
            Serial.print(" Distance results:");
            Serial.print(results.distance_mm[x + y]);
            distance.push_back(results.distance_mm[x+y]); 
            for (int i=0; i<=length; i++){
                average_dist = distance[i]+average_dist;
            }
            average_dist = average_dist/64;
        }
        Serial.println();
        }
        size_t length = distance.size();
              
        
        Serial.print("Average distance:");
        Serial.print(average_dist);
        Serial.println();
    }
    }

    if (average_dist < 100){
            digitalWrite(buzzerPin, HIGH); // Turn the buzzer on
            delay(100);                // Wait for 1 second
            digitalWrite(buzzerPin, LOW);  // Turn the buzzer off
            delay(100);                // Wait for 1 second
    }else if (average_dist < 500){
            digitalWrite(buzzerPin, HIGH); // Turn the buzzer on
            delay(250);                // Wait for 1 second
            digitalWrite(buzzerPin, LOW);  // Turn the buzzer off
            delay(250);                // Wait for 1 second
    } else if (average_dist > 500){
            digitalWrite(buzzerPin, LOW); // Turn the buzzer off
        }
    average_dist = 0;

  delay(5); //Small delay between polling
}