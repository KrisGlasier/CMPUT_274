/*
* Task: Design and impliment a FSM (Finite state machine) that recognizes
* valid canadian postal codes, allows for leading, trailing spaced and
* spaces between the two "parts" and is not case sensitive.
*/
#include <Arduino.h>

/* Built in codes:

( "bool isalpha(char c);" ) returns true if a character
received is 'a' to 'z' or 'A' to 'Z' if not, returns false.

( "bool isdigit(char c)" ) returns true if character is 0-9, false if not.
*/

// Verify if str gives valid postal code form. Allow trailing,
// leading spaces, and a space between two halves.

bool verifyPostalCode(char *str){
    enum State { L1, N1, L2, N2, L3, N3, DONE, ERR  }; // Our states.
    State currentState = L1;
    // Loop that processes one letter at a time.

    int i = 0;
    while(1){
        // get the next letter and increment.
        char c = str[i];
        i++;

        if(c == '\0'){
            break;
        }

        // FSM control Check if it's in  L-N-L N-L-N form...
        if (currentState == L1 && c == ' ')   { ; }
        else if (currentState == L1 && isalpha(c)) { currentState = N1;   } // L
        else if (currentState == N1 && isdigit(c)) { currentState = L2;   } // N
        else if (currentState == L2 && isalpha(c)) { currentState = N2;   } // L
        else if (currentState == N2 && c == ' ')   { ; }
        else if (currentState == N2 && isdigit(c)) { currentState = L3;   } // N
        else if (currentState == L3 && isalpha(c)) { currentState = N3;   } // L
        else if (currentState == N3 && isdigit(c)) { currentState = DONE; } // N
        else if (currentState == DONE && c == ' ') { ; }

        else {
            currentState = ERR;
        }
    }

    return currentState == DONE;
}

void testSinglePostalCode(char *str, bool expected){
    if(verifyPostalCode(str) != expected){
        Serial.print("Error in test: "); Serial.println(str);
        Serial.print("Expected: "); Serial.println(expected);
        Serial.print("Received: "); Serial.println(!expected);
        Serial.println("----------------------");
    }
}
void testPostalCode(){
    testSinglePostalCode("R2D 2A3", true);
    testSinglePostalCode("R2D   2A3", true);
    testSinglePostalCode("  R2D 2A3  ", true);
    testSinglePostalCode("R2D 2Aa", false);
    testSinglePostalCode("R2D #A3", false);
    testSinglePostalCode("", false);

    Serial.print("Test complete!");
}

int main(){
    init();
    Serial.begin(9600);

    testPostalCode();

    while(1){}
    Serial.end();
    return 0;
}
