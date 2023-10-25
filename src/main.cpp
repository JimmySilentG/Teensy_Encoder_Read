//#include <Arduino.h>
#include <Encoder.h>

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder Hall_One(2, 3);
//   avoid using pins with LEDs attached

void setup() {
  Serial.begin(9600);
  Serial.println("Encoder Testing...");
}

long position  = 0; //testing github

void loop() {
  long newPosition;
  newPosition = Hall_One.read();
  if (newPosition != position){
    Serial.println(newPosition);
    position = newPosition;
  }
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    //Serial.println(Hall_One.read());
    Serial.read();
    Serial.println("Reseting Count");
    Hall_One.write(0);
  }
}