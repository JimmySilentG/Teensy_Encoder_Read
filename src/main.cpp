//#include <Arduino.h>
#include <Encoder.h>

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder Hall_One(2, 3);
//Encoder Hall_Two(4, 5);
//   avoid using pins with LEDs attached

void setup() {
  Serial.begin(115200); //teensy just uses maximum the usb allows so it doesnt matter what goes here
}

unsigned long axis1 = 0;
//unsigned long axis2 = 0;
unsigned long counter = 0; //used for programmatically incrementing encoder while not hooked up
byte b1[1]; //temporary buffers used only to step through the incoming message and find the start bytes
byte b2[1];
float DutyCycleRecieved; //create variable to store duty cycle command calculated from RPI
float finished = 50.0001;
int count; //variable used to create logic same as ROS side to prevent excessively bad packets

void loop() {
  axis1 = Hall_One.read() + 2147483648;
  //axis2 = Hall_Two.read() + 2147483648;
  axis1 = axis1 + floor(counter/500); // for manually incrementing values when I dont have motors wired in
  //axis2 = axis2 + floor(counter/500); // same here ^^
  //Serial.write(0xAA); //send two back to back 170 bytes so RPI knows a new transmission is coming in
  //Serial.write(0xAA);
  //Serial.write((uint8_t*)&axis1, sizeof(axis1)); //split up the axis1 variable into byte-sized pieces and send over serial
  //Serial.write((uint8_t*)&axis2, sizeof(axis2));
  b1[0] = 0.0;
  b2[0] = 0.0;
  if (Serial.available() >= 6) { //if there are 6 or bytes available to read
    //I think this all needs to be one function
    Serial.readBytes((char*)b1, 1);
    Serial.readBytes((char*)b2, 1); //read in first two bytes of the transmission to see if they match start sequence
    while (!((b1[0] == 0xAA) and (b2[0] == 0xAA))){ //while the first two bytes are not the 170 start bytes
      b1[0] = b2[0]; //this logic should create the improved step through for better performance that i was looking for
      Serial.readBytes((char*)b2, 1); 
      count++;
      if (count >= 5){
        while (Serial.available() > 0){
          Serial.read(); //read off any incoming data to nothing to clear it
        }
        count = 0; //if we get continuously bad packets this will go on forever... putting this all in a function with a return will solve this but not in the mood
      }
    }
    Serial.readBytes((char*)&DutyCycleRecieved, 4); //next four bytes should be the good floating point duty cycle
    float DutyDouble = DutyCycleRecieved * 2.0;
    Serial.write((uint8_t*)&DutyDouble, sizeof(DutyDouble));
  }
  counter++;
delay(2); //delay 2 millisecond each send cycle for 500hz send rate "not sure why it was 0.002 which was maxing out"
}