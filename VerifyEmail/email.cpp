#include <Arduino.h>

/*
  FSM - Finite State Machine
  Checks to see if an email is in the form of
  Decimal.Seperated.Name@Decimal.Seperated.Host.domain
  Examples:
    John.Doe@Example.ca
    John@Example.com
    John@my.example.ca
*/

bool verifyEmail(char *str){
    enum State { CP1, CP2, SP1, SP2, SP3, SP4, ERR  }; // Our states.
    State currentState = CP1;

    int i = 0;
    while(1){
        // get the next letter and increment.
        char c = str[i];
        i++;

        if(c == '\0'){
            break;
        }

        // // FSM control Check if it's in  L-N-L N-L-N form...
        if      (currentState == CP1 && (c == '_' || isalpha(c)) )   { currentState = CP2; }
        else if (currentState == CP2 && ( c == '_' || isdigit(c) || isalpha(c)) ){ ; }
        else if (currentState == CP2 && c == '.')   { currentState = CP1; }
        else if (currentState == CP2 && c == '@') { currentState = SP1; }

        else if (currentState == SP1 && isalpha(c)){ currentState = SP2; }
        else if (currentState == SP2 && isalpha(c)) { ; }
        else if (currentState == SP2 && c == '.') { currentState == SP3; }

        else if (currentState == SP3 && isalpha(c)) { currentState = SP4; }
        else if (currentState == SP4 && isalpha(c)) { ; }
        else if (currentState == SP4 && c == '.') {currentState = SP3;}

        else {
            currentState = ERR;
        }
    }
    return currentState == SP4;
}


int main(){
    init();
    Serial.begin(9600);

    Serial.print(verifyEmail("kris.glasier@gmail.com"));

    while(1){ }
    Serial.end();
    return 0;
}
