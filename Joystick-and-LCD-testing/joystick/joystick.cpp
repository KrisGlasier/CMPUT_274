/* Simple Joystick Example */

#include <Arduino.h>

/* Joystick Definitions */

const int VERT = 0;  // analog input
const int HORIZ = 1; // analog input
const int SEL = 9;   // digital input

void setup() {

  /* Initialize Joystick */

  /* make the SEL line an input and use internal pull-up resistor */

  pinMode(SEL,INPUT);
  digitalWrite(SEL,HIGH);

  /* Initialize Serial Connection */

  Serial.begin(9600);
}

void loop()
{
  int vertical, horizontal, select;

  /* read values from the joystick */

  vertical = analogRead(VERT);      // will be 0-1023
  horizontal = analogRead(HORIZ);   // will be 0-1023
  select = digitalRead(SEL);        // HIGH if not pressed, LOW otherwise

  /* print out the values */

  Serial.print("VER= ");
  Serial.print(vertical,DEC);
  Serial.print(" HOR= ");
  Serial.print(horizontal,DEC);
  Serial.print(" SEL= ");
  Serial.print(select,DEC);
  Serial.println();
}
