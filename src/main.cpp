//#include <Arduino.h>
#include <Encoder.h>

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder Hall_One(2, 3);
Encoder Hall_Two(4, 5);
//   avoid using pins with LEDs attached

void setup() {
  Serial.begin(115200); //teensy just uses maximum the usb allows so it doesnt matter what goes here
}

unsigned long axis1 = 0;
unsigned long axis2 = 0; 

void loop() {
  axis1 = Hall_One.read() + 2147483648;
  axis2 = Hall_Two.read() + 2147483648;
  Serial.write(0xAA); 
  Serial.write(0xAA);
  Serial.write((uint8_t*)&axis1, sizeof(axis1));
  Serial.write((uint8_t*)&axis2, sizeof(axis2));
delay(0.002); //delay 2 millisecond each send cycle for 500hz send rate
}