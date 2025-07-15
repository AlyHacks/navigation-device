const int trigPin = 8; //this sets up the trigger pin, which emits the soundwave for detection
const int echoPin = 7; //this pin recieves the signal, or sound, and tells arduino how long it took
const int ledPin = 2; //this is the pin for LED


void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600); //sends data in 9600 bits/sec or 9600 baud
}

void loop() {
  long duration; //variable duration, long is extra long integer :)
  float distance; //variable distance, type is float

  //digitalWrite is a function that allows to set pins on HIGH or LOW
  digitalWrite(trigPin, LOW); //sets the voltage to LOW or 0 = pre-setting for the actual program 
  delayMicroseconds(2); //pauses program for 2 microseconds, tiny buffer for pre-setting
  digitalWrite(trigPin, HIGH); //emits sound pulse
  delayMicroseconds(10); //wait for 10 microseconds to allow for pulses
  digitalWrite(trigPin, LOW); //ends the 10 microsecond pulse

  duration = pulseIn(echoPin, HIGH); //measures the amount of time echoPin stays HIGH
  distance = 0.0343*duration/2; //calculates distance using v=lambda*f, where v=d/t, so d/t = 343m/s
  Serial.print("Distance: "); //prints distance: x cm
  Serial.print(distance);
  Serial.println(" cm"); //println here so that each distance reading isn't all on the same line, ln makes sure it prints, but the next string is on a different line
  
  if (10 < distance && distance < 30){
    digitalWrite(ledPin, HIGH); //led stays on if there is an object that is less than 30 cm away
  } else if (distance <= 10){    //led blinks if 10 cm or less away
      digitalWrite(ledPin, HIGH);
      delayMicroseconds(800);
      digitalWrite(ledPin,LOW);
    
  } else {
      digitalWrite(ledPin, LOW); //if faether than 30 cm turn off led
  }


  delay(500); // allows for loop to rest for 500 miliseconds



}