/*
 * This function does the follwoing:
 *      1. Counts the length of a string.
 *      2. Prints the string backwards.
 *
 * Additional functionality:
 *      1. Requests user input for the string they wish to have
 *            counted and printed backwards.
 *      2. Repeats after each itteration so user does not have to restarts
 *            arduino themselves.
 */
#include <Arduino.h>

char str[4000];
int lengthOfString;

/*  This function takes a string, automatically null
*  terminated, from a user interface. */
void user_input();

/*  This function takes a null terminated string, str, and determins
*  it's length, printing to the serial montior. */
int getStringLength( char* str );

/*  This function takes a null terminated string, str, and prints it
*  backwards to the serial monitor. */
void printBackwards( char* str );


int main(){
    init();
    Serial.begin(9600);

    Serial.println("Please input a string, hit enter to finish: ");

    while(1){
        if(Serial.available() != 0){
            // Takes a string of characters from user.
            user_input();
            // Computes length of null-terminated string
            lengthOfString = getStringLength(str);
            // Prints null-terminated string backwards
            printBackwards(str);
        }
    }
    while(1){}
    return 0;
}

void user_input(){
    int i = 0;
    while(1){
        while(Serial.available() == 0) { }
        char incomingByte = Serial.read();

        if(incomingByte == 13){ // if user hits enter (ASCII code 13),
            str[i] = 0;         // terminate string, and break while loop.
            break;
        }
        if(incomingByte == 8){ // If user hits backspace, removes character
            i = i-1;           // from serial-mon
            Serial.write("\b ");
        }
        else{
            str[i] = incomingByte;
            ++i;
        }
        Serial.write(incomingByte);
    }
    Serial.println("");
}

int getStringLength( char* str ){
    int q = 0;
    // Checks if string is terminated, if not, keep counting.
    for(int i=0; i<0x7FFF; i++){
        if(str[q] == 0){
            break;
        }
        else{
            q++;
        }
    }

    Serial.print("Your string contains "); Serial.print(q);
    Serial.print(" characters, ");

    // Returns value to use in other functions.
    return q;
}

void printBackwards( char* str ){
    Serial.println("and printed backwards is: ");
    /* Starts at right-most end of string and prints character by character. */
    for(int i = lengthOfString; i > 0; --i){
        Serial.print(str[i-1]);
    }
    // Additional space for readability in main loop.
    Serial.println("\r\n");
    Serial.println("You may type another string if you wish.\r\n\r\n");
}
