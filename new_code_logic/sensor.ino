



// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
//#include <TimeLib.h>




const int ledPin = 4;




// Create a structured object
float received_distance = 10000;


int ontime = 500;
int distMult = 5;


#define bignum 10000000;


unsigned long next_pulse = millis() + bignum;
unsigned long pot_next_pulse = next_pulse;
unsigned long cur_time;
unsigned long end_pulse;












// Callback function executed when data is received
void OnDataRecv(const esp_now_recv_info * mac, const uint8_t *incomingData, int len) {
 memcpy(&received_distance, incomingData, sizeof(received_distance));
 Serial.print("Data received: ");
 Serial.println(len);
 Serial.print("Distance:");
 Serial.println(received_distance);
 Serial.println();




}








void setup() {
 pinMode(ledPin, OUTPUT);




 // Set up Serial Monitor
 Serial.begin(115200);
 // Set ESP32 as a Wi-Fi Station
 WiFi.mode(WIFI_STA);








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


}


// At one point in time we start the loop




//            X
// start   next pulse   end pulse
//   |        |           |
// ------------------------------


//we take next pulse off the list and make it a huge number after that pulse ends at end pulse


// if there is a distance received then the code will predict the potential next pulse, if that potential next pulse is going to happen BEFORE the actual next pulse then it replaces the next pulse. But if the potential next pulse is predicted to be AFTER the next pulse then the pulse will jsut happen at next pulse.






void loop() {
 cur_time = millis();// everymillis we check the time in the loop


 if(cur_time > next_pulse){ // If the time of the current pulse that is happnening in the moment is passing(interrupting) the time of the next pulse then it turns the buzzer on for that next pulse and also sets end pulse to next pulse plus a new really big variable
   //start buzz
   digitalWrite(ledPin, HIGH);
   end_pulse = next_pulse + ontime;
   next_pulse = cur_time + bignum;
 }
 if(cur_time > end_pulse){ //Checking when the
   //end buzz
   digitalWrite(ledPin, LOW);
   //digitalWrite(ledPin, LOW);
 }




 if (received_distance < 50){
   //per_end_pulse = cur_time + ontime;
   pot_next_pulse = ontime + 10 + cur_time; //potential next pulse
 }
 else if (received_distance < 200){
   pot_next_pulse = distMult*(received_distance/5) + ontime + cur_time;
 }
 else if (received_distance <= 1000 && received_distance >= 200)  {    //led blinks if 10 cm or less away
   pot_next_pulse = distMult*(received_distance/10) + ontime + cur_time;
 }
 else if (received_distance <= 3000) {
   pot_next_pulse = distMult*(received_distance/10) + ontime + cur_time;
 }
 else {
   //digitalWrite(ledPin, LOW);


 }


 if(pot_next_pulse < next_pulse){
   next_pulse = pot_next_pulse;
 }
 //delayMicroseconds(300);




}

