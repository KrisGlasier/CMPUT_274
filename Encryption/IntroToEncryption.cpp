/** Chat: Two Arduinos are connected
through the Serial3 port.
Whatever one types on one of them,
shows up on the screen of the other.
*/
#include <Arduino.h>


/* Calculates (a**b) mod m
a: base, 32 bits long
b: exponent, 32 bits long
m: modulus, 16 bits long (0<m<2^16)
Running time: O(1) [constant running]
*/
uint32_t fast_pow_mod( uint32_t a, uint32_t b, uint32_t m ) {
    uint32_t r = 1 % m; // will store the result at the end
    uint32_t p = a % m; // will store a^{2^i} % m for i=0,1,...
    for (int i=0; i<32; ++i) {
        if ( (b & (1ul<<i))!=0 ) { // check i-th bit of b
            r = (r*p) % m;
        }
        p = (p*p) % m;
    }
    return r;
}

/* Calculates (a**b) mod m
a: base, 32 bits long
b: exponent, 32 bits long
m: modulus, 16 bits long (0<m<2^16)
Warning: This will be extremely slow when b is large
Running time: O(b).
*/
uint32_t pow_mod( uint32_t a, uint32_t b, uint32_t m ) {
    uint32_t result = 1;
    uint32_t i;
    a = a % m;
    for (i=1; i<=b; ++i) {
        // 0<=result<=m-1<2^16
        // 0<=a<=m-1<2^16
        // hence: result*a fits 32 bits! no overflow!
        result = (result * a) % m;
    }
    return result;
}

/** Tests pow_mod with the first three arguments against the fourth argument */
void test_pow_mod_case( uint32_t a, uint32_t b, uint32_t m, uint32_t exp_result) {
    uint32_t result = fast_pow_mod( a, b, m );
    if (exp_result!=result) {
        Serial.println("FAIL!");
        Serial.print("Base:" ); Serial.println(a);
        Serial.print("Exponent: "); Serial.println(b);
        Serial.print("Modulus: "); Serial.println(m);
        Serial.print("Expected result:"); Serial.println(exp_result);
        Serial.print("Result from pow_mod:"); Serial.println(result);
    }
}

void timing_pow_mod() {
    uint32_t base = 5;
    uint32_t modulus = 37;
    uint32_t exponents[] = {10,100,1000,10000,100000,1000000};
    const int n_exponents = 6;
    for (int i=0; i<n_exponents; ++i) {
        unsigned long start = millis();
        fast_pow_mod( base, exponents[i], modulus );
        unsigned long end = millis();
        Serial.print("Exponent: ");
        Serial.println(exponents[i]);
        Serial.print(" Time elapsed: ");
        Serial.println( end-start );
    }
}

void test_pow_mod() {
    test_pow_mod_case(5, 10, 37, 30);
    test_pow_mod_case(5, 8,  37, 16);

    // more test cases: big values
    // 1. big exponents
    test_pow_mod_case(5, 100,  37, 7);
    test_pow_mod_case(5, 1000,  37, 7);
    test_pow_mod_case(5, 10000,  37, 7);
    test_pow_mod_case(5, 100000,  37, 7);
    test_pow_mod_case(5, 100001,  37, 35);
    // 2. big base
    test_pow_mod_case(2374237, 10,  37, 16);
    test_pow_mod_case(2374237, 100,  37, 16);
    test_pow_mod_case(2374237, 10000,  37, 16);

    test_pow_mod_case(1, 0,  1, 0);
    Serial.println("Test finished");
    // 3. big modulus

}

void setup() {
    init();
    Serial.begin(9600);
    Serial3.begin(9600);
}

/* Get unsigned integer number from your the PC and return it */
uint32_t get_number_from_PC() {
    //TODO: Fix!!!
    return 8;
}

int main() {
    setup();
    timing_pow_mod();
    //test_pow_mod();
    while (true) {
    }
    uint32_t prime = 37; // Diffie-Hellman prime
    uint32_t generator = 5; // Diffie-Hellman generator
    uint32_t private_key = 10; // 1<=private_key<=prime-1
    uint32_t public_key = 0; // to be computed later: (generator**private_key) mod prime
    char     shared_secret_key = 0; // to be computed later

    public_key = pow_mod(generator, private_key, prime);
    // Share public key with partner:
    Serial.print("Tell your partner this number: ");
    Serial.println(public_key);
    Serial.print("Please enter the number you got from your partner: ");
    uint32_t partners_public_key = get_number_from_PC();
    shared_secret_key = pow_mod(partners_public_key, private_key, prime);

    Serial.print("Hello!!");
    while (true) {
        // PC Keystrokes -> other Arduino
        if (Serial.available()>0) {
            char c = Serial.read();
            // echo character received:
            Serial.print(c);
            c = c ^ shared_secret_key; // encryption
            Serial3.write(c);
        }
        // Other Arduino input -> PC Screen
        if (Serial3.available()>0) {
            char c = Serial3.read();
            c = c ^ shared_secret_key; //decryption
            // show received, decrypted character:
            Serial.print(c);
        }
    }

    Serial3.end();
    Serial.end();
    return 0;
}
