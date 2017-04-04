/*
Binary Counter - Assignment 1, Lab 5, Counter

Create a five (5) digit binary counter with a button to
increase value and a button to decrease value.

250 ms delay between light changes for visible counting while button held.
*/
#include <Arduino.h>


// change these 5 variables accordingly to how many LEDs you would like.
int n=5;
int LEDpin[5]   = { 13, 12, 11, 10,  9 }; // Pins for LED
int LEDstate[5] = {  0,  0,  0,  0,  0 }; // Zero array.
int divide[5]   = {  1,  2,  4,  8, 16 }; // 2^n for the nth LED
int max = 32;                             // (2^n) where n = number of LEDs

int ButtonPin[2] = { 6, 7 };
int counter = 0;
int i;
int button[2] = { 0, 0 };

void setup(){
    init();

    for(i=0; i < n; i++){
        pinMode(LEDpin[i], OUTPUT);
    }

    for(i=0; i < 2; i++){
        pinMode(ButtonPin[i], INPUT);
        digitalWrite(ButtonPin[i], 1);
    }

    Serial.begin(9600); // enable serial port communications
}


int main() {
    setup();

    while(1){
        button[0] = digitalRead(ButtonPin[0]);
        button[1] = digitalRead(ButtonPin[1]);

        if(button[0] == 0){    ++counter;    }

        if(button[1] == 0){         // Decrease Value button
            if(counter == 0) {    counter = max + 1 ;    }
            --counter;
        }

        for(int i=0; i<n; i++){   // Turn lights on to mimic binary digits.
            LEDstate[i] = (counter/divide[i])% 2 ;
            digitalWrite(LEDpin [i],LEDstate[i]);
        }
        delay(250);
    }
    return 0;
}
