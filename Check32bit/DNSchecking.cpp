/*
 * Bit Manipulation - Exercise 2
 *  By Kristopher Quaife-Glasier
 *        ECE 274 LBL EA1
 * -----------------------------
 *   Summary:

 *   This .cpp file allows the user to do two things.
 *        - Path A) Allows the user to determine the number of
 *              bits a large postiive number (0 to (2^32) - 1, inclusive)
 *              would take to represent.
 *        - Path B) Allows the user to determine one of the four
 *              octets in a given 32-bit integar.
 *
 *   Extras:
 *        - Once program is finished, has 5 second delay before returning
 *              to the main menu.
 *        - Has a function incase of incorrect input, and restarts the program.
 *
 *  PROBLEM: I call "main" in this several times. Don't do that..
 *
 */

#include <Arduino.h>

  unsigned long   user_input_a();
  uint32_t        getOctet(uint32_t ipAddress, uint32_t octet);
  void            Oopsies();
  void            redir();
  int             numBitsUnsigned();

  unsigned long   cookie          =    1 << 31;
  uint32_t        masking         =  0xF;
  uint32_t        ipAddress       =    0;
  uint8_t         octet           =    0;
  char            initialInput    =    0;
  int             i               =    0;
  int             Transition      = 5000;

int main() {                    // Main. Directory to path A or B.
    init();
    Serial.begin(9600);

    for(i=0; i==1; i++){
        // Problems with part b when variables are not reset. Not sure
        // if it can be fixed another way? In a for loop for easy folding.
        uint32_t        masking = 0xF;
        uint32_t        ipAddress = 0;
        uint8_t         octet = 0;
        char            initialInput = 0;
        int             i = 0;
    }
    // Pick the path you'd like to take. a or b.
    Serial.print("Would you like to test part... \n\r"
    "a) Determining minimum of bits for a value?\n\r"
    "b) See a specific octet in an unsigned long value?\n\r"
    "Your selection: ");

    // Wait for user input
    while(Serial.available() == 0) { }
    initialInput = Serial.read();
    Serial.println(initialInput);
    Serial.println("");

    // To change directory depending on input. Invalid = restart.
    if(initialInput ==  97){
        numBitsUnsigned();
    }
    if(initialInput == 98){
        getOctet(ipAddress, octet);
    }
    else{
        Oopsies();
    }

    while(1){}
    return 0;
}

unsigned long  user_input_a(){  // Path A, Function 1 of 2.
    const int MAX_LENGTH = 128;
    char line[MAX_LENGTH + 1];

    Serial.print("Please enter an unsigned long vaue: ");

    while(1){           // Reads input, echos, and converts to array.
        while(1){
            while(Serial.available() == 0) { }
            int incomingByte = Serial.read();

            if(incomingByte == 13){
                line[i] = 0;
                break;
            }

            char ch = incomingByte;
            Serial.print(ch);
            line[i] = incomingByte;
            i++;
        }
        Serial.println("");
        unsigned long result = atol(line);
        // If value is greater than 2^32, error. Restart.
        if(result > 0xFFFFFFFF && result < 0){
            Oopsies();
        }
        return result;
    }
}

int numBitsUnsigned(){          // Path A, Function 2 of 2.
    unsigned long r = user_input_a();

    for(i=32; i>0;--i){
        if(r/pow(2,i) >=1) break;
    }
    Serial.print("Number of bits required: "); Serial.print(i+1);

    redir();            // Redirectory to main menu
}

uint32_t getOctet(uint32_t ipAddress, uint32_t octet){ // Path B, Only Function
    const int MAX_LENGTH = 128;
    char line[MAX_LENGTH + 1];

    Serial.print("Please enter a uint32_t value: ");

    while(1){       // Retreiving uint32_t value.
        while(Serial.available() == 0) { }
        ipAddress = Serial.read();

        if(ipAddress == 13){
            line[i] = 0;
            break;
        }

        char ch = ipAddress;
        Serial.print(ch);
        line[i] = ipAddress;
        i++;
    }

    Serial.println("");
    Serial.print("Please select your octet (3, 2, 1, or 0): ");

    while(1){       // Find octet and relay back it's decimal equivilent.
        while(Serial.available() == 0) { }

        octet = Serial.read();
        char ch = octet;
        Serial.println(ch);
        uint32_t ipAddress = atol(line);    // Convert back to int.

        if(ipAddress > 0xFFFFFFFF && ipAddress < 0){
            Serial.print("Invalid uint32_t.");
            Oopsies();
        }

        i = octet-48;           // Quick way to get octet from char.

        if(i > 3 && i < 0){
            Serial.print("Invalid Octet.");
            Oopsies();
        }

                        // Bit masking without the use of logical AND.
        ipAddress = ipAddress << 8*i;
        ipAddress = ipAddress >> 24;

        Serial.print("Your octet is: ");
        Serial.println(ipAddress);

        redir();
    }
}

void Oopsies(){                 // If invalid input, restart program.
    Serial.print("\n\r""\n\r\tOopsies! Looks like you've entered \n\r\t"
    "something in wrong!! Have you consiered \n\r\t"
    "any of the following?\n\r\t"
    "a) Please use lowercase letters when selecting from the menu.\n\r\t"
    "b) Is your number positive and below 2^32?\n\r\t"
    "c) You inserted an invalid input such as a symbol, or invalid letter?"
    "\n\r"" \n\rRedirecting to main menu.");
    for(i=0; i<5; i++){         // Just for some live action!
        Serial.print(" .");
        delay(1000);
    }
    Serial.println("");
    main();
}

void redir(){                   // Back to main menu after competing A or B
    Serial.print("\n\r""\n\rYou'll be reidrected to the main"
    " menu shortly.");
    for(i=0; i<5; i++){
        Serial.print(" .");
        delay(1000);
    }
    Serial.println("");
    main();
}
