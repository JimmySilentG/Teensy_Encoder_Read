//#include <Arduino.h>
#include <Encoder.h>

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder Hall_One(2, 3);
//   avoid using pins with LEDs attached
IntervalTimer sersendTimer; //https://www.pjrc.com/teensy/td_timing_IntervalTimer.html

unsigned long axis1 = 0;
unsigned long counter = 0; //used for programmatically incrementing encoder while not hooked up
byte b1[1]; //temporary buffers used only to step through the incoming message and find the start bytes
byte b2[1];
float DutyCycleRecieved; //create variable to store duty cycle command calculated from RPI
int count; //variable used to create logic same as ROS side to prevent excessively bad packets
//int baud;
int DutyCycleCommand;

void sendAxis() {
  axis1 = Hall_One.read() + 2147483648;
  //axis1 = axis1 + floor(counter/500); // for manually incrementing values when I dont have motors wired in
  Serial.write(0xAA); //send two back to back 170 bytes so RPI knows a new transmission is coming in
  Serial.write(0xAA);
  Serial.write((uint8_t*)&axis1, sizeof(axis1)); //split up the axis1 variable into byte-sized pieces and send over serial
  //Serial.println(axis1 - 2147483648);
}

void setup() {
  //MUST SET BAUDRATE ON DEVICE THROUGH TERMINAL,TEENSY USES WHATEVER THE COMPUTER OR RPI USES
  //OH MY DAYS THE ENCODER VOLTAGE WAS TOO LOW, WE GOT GOOD SIGNALS NOW BOYS
  Serial.begin(115200); //teensy just uses maximum the usb allows so it doesnt matter what goes here
  sersendTimer.begin(sendAxis, 4000); //interrupt timer every 4000 microseconds for 250hz send rate
  b1[0] = 0.0;
  b2[0] = 0.0;
  //baud = Serial.baud();
}

//pin 4 is dir 5 is speed

void loop() {
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
    DutyCycleCommand = round(DutyCycleRecieved * (256.0/100.0)); //need to convert floating duty cycle to integer duty cycle from 0-256
    if (DutyCycleCommand <= 0) { //abs of zero is zero so it doesnt matter what postion direction pin is in
      analogWrite(4, 256); //writing direction pin low for negative duty cycles
      analogWrite(5, (8 + abs(DutyCycleCommand))); //increase these values by 10 because under 10 the motor cant overcome friction but that could f with pid tune
    } else { //duty cycle commanded is positive
      analogWrite(4, 0); //writing direction pin high for positive duty cycles
      analogWrite(5, (8 + DutyCycleCommand));
    }
    //if we arent recieving anything then set the pins low so we dont break anything
    //analogWrite(5,0);
  }
//delayNanoseconds(250000); //delay (does not impact interrupts)*/
}