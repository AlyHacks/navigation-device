#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <SparkFun_VL53L5CX_Library.h> //http://librarymanager/All#SparkFun_VL53L5CX
#include <Wire.h>
#include <esp_now.h>
#include <WiFi.h>
#include <list>
#include <unordered_set>


#define I2C_SDA D4
#define I2C_SCL D5


const int buzzerPin = D7; // GPIO pin connected to the buzzer



SparkFun_VL53L5CX sensor;
VL53L5CX_ResultsData results; // Result data class structure, 1356 byes of RAM

int imageResolution = 0; //Used to pretty print output
int imageWidth = 0; //Used to pretty print output

//later, find a simpler way to calculate averages of quadrants
//also double check these numbers are even correct
std::unordered_set<int> top_left = {1, 2, 3, 4, 9, 10, 11, 12, 17, 18, 19, 20, 25, 26, 27, 28}; 
std::unordered_set<int> top_right = {5, 6, 7, 8, 13, 14, 15, 16, 21, 22, 23, 24, 29, 30, 31, 32};
std::unordered_set<int> bottom_left = {33, 34, 35, 36, 41, 42, 43, 44, 49, 50, 51, 52, 57, 58, 59, 60}; 
std::unordered_set<int> bottom_right = {37, 38, 39, 40, 45, 46, 47, 48, 53, 54, 55, 56, 61, 62, 63, 64};

//initiating variables to use later for measuring average distance of the 4 quadrants
int average_dist_tl;
int average_dist_tr;
int average_dist_bl;
int average_dist_br;



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
    int average_dist = 0;
 

    //Poll sensor for new data
    if (sensor.isDataReady() == true)
    {
    if (sensor.getRangingData(&results)) //Read distance data into array
    {
        //The ST library returns the data transposed from zone mapping shown in datasheet
        //Pretty-print data with increasing y, decreasing x to reflect reality
        for (int y = 0 ; y <= imageWidth * (imageWidth - 1) ; y += imageWidth) {
          for (int x = imageWidth - 1 ; x >= 0 ; x--)
          {
              Serial.print(" Distance results:");
              Serial.print(results.distance_mm[x + y]);
              average_dist += results.distance_mm[x + y];

              if(top_left(x+y) != top_left.end()) { //if x+y ("index" value in sensor readings) is part of the top left quadrant, add to quadrant's distance counter
                average_dist_tl += results.distance_mm[x+y];
              }
              else if(top_right(x+y) != top_right.end()) {
                average_dist_tr += results.distance_mm[x+y];
              }
              else if(bottom_left(x+y) != bottom_left.end()) {
                average_dist_bl += results.distance_mm[x+y];
              }
              else if(bottom_right(x+y) != bottom_right.end()) {
                average_dist_br += results.distance_mm[x+y];
              }
          }
          Serial.println();
        }              

        average_dist = average_dist/64;
        Serial.print("Average distance:");
        Serial.print(average_dist);
        Serial.println();

        average_dist_tl = average_dist_tl/16;
        Serial.print("Average distance of top left:");
        Serial.print(average_dist_tl);
        average_dist_tr = average_dist_tr/16;
        Serial.print("Average distance of top right:");
        Serial.print(average_dist_tr);
        average_dist_bl = average_dist_bl/16;
        Serial.print("Average distance of bottom left:");
        Serial.print(average_dist_bl);
        average_dist_br = average_dist_br/16;
        Serial.print("Average distance of bottom right:");
        Serial.print(average_dist_br);
    }
    }

    if (average_dist < 200){
            digitalWrite(buzzerPin, HIGH); // Turn the buzzer on
            delay(100);                // Wait for 1 second
            digitalWrite(buzzerPin, LOW);  // Turn the buzzer off
            delay(100);                // Wait for 1 second
    }else if (average_dist < 500){
            digitalWrite(buzzerPin, HIGH); // Turn the buzzer on
            delay(250);                // Wait for 1 second
            digitalWrite(buzzerPin, LOW);  // Turn the buzzer off
            delay(250);                // Wait for 1 second
    } else {
            digitalWrite(buzzerPin, LOW);
            delay(2000); // Turn the buzzer off
            Serial.println("distance is > 500");
        }
    average_dist = 0;

  delay(5); //Small delay between polling
}