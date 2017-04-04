/* Printing in binary */
#include <Arduino.h>

/*
Prints the binary digits of a number.
Takes an unsigned integer as an arguement,
and prints it's binary digits in a new line.
*/

void test_printBinary();
void printBinary(unsigned long a);
unsigned long get_input();
char len;
int i;

int main() {
    init();
    Serial.begin(9600); // enable serial port communications

    unsigned long r = get_input();
    printBinary(r);
    //    test_printBinary();
    Serial.print("Cheers!");


    while (true) {}
    return 0;
}

unsigned long get_input(){
    const int MAX_LENGTH = 128;
    char line[MAX_LENGTH + 1];
    while(1){
        while(Serial.available() == 0) { }
        // read the incoming byte:
        int incomingByte = Serial.read();
        // say what you got:
        // Serial.print("I received: ");
        // Serial.println(incomingByte);
        if(incomingByte == 13){
            line[i] = 0;
            break;
        }
        char ch = incomingByte;
        Serial.print(ch);
        line[i] = incomingByte;
        i++;
    }
    Serial.println(" ");
    Serial.print("User Input: "); Serial.println(line);
    int result = atoi(line);
    Serial.print("After Converstion: "); Serial.println(result);
    return result;
}

void test_printBinary(){
    unsigned int a=1;
    for(int i=0; i<16; i++){
        Serial.print("Input ");
        Serial.print(a);
        //len=char(Serial.print(a));
        //len.length();
        Serial.print(" Binary Expansion: ");
        printBinary(a);
        a *= 2;
    }
}

void printBinary(unsigned long a){
    unsigned long long b = 1 << 16;
    for (int i=0; i<32; ++i) {
        if(i % 4 == 0) Serial.print(" ");
        if ((a&b)!=0) {
            Serial.print(1);
        }
        else {
            Serial.print(0);
        }
        b /= 2; // b = b / 2 or b = 1 >> 1
    }
    Serial.println(" ");
}
