/* Simple Image Drawing
*
* Draws an image to the screen.  The image is stored in "parrot.lcd" on
* the SD card.  The image file contains only raw pixel byte-pairs.
*/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>

#include "lcd_image.h"

// standard U of A library settings, assuming Atmel Mega SPI pins
#define SD_CS    5  // Chip select line for SD card
#define TFT_CS   6  // Chip select line for TFT display
#define TFT_DC   7  // Data/command line for TFT
#define TFT_RST  8  // Reset line for TFT (or connect to +5V)

#define JOY_VERT_ANALOG 0
#define JOY_HORIZ_ANALOG 1

#define TFT_WIDTH 128
#define TFT_HEIGHT 160

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

lcd_image_t map_image = { "yeg-big.lcd", 2048, 2048 };

uint16_t icol = 1024;
uint16_t irow = 1024;

int update = 0;

void scanJoystick();
void updateScreen();

void setup(void) {
    init();

    Serial.begin(9600);

    //tft.initR(INITR_REDTAB);   // initialize a ST7735R chip, red tab
    tft.initR(INITR_BLACKTAB);   // initialize a ST7735R chip, black tab
    //tft.initR(INITR_GREENTAB); // initialize a ST7735R chip, green tab

    Serial.print("Initializing SD card...");
    if (!SD.begin(SD_CS)) {
        Serial.println("failed!");
        return;
    }
    Serial.println("OK!");

    // clear to yellow
    //tft.fillScreen(tft.Color565(0xff, 0xff, 0x00));

    // fill the whole screen with a part of the picture of Edmonton
    // the upper-left corner of the screen should be exactly
    // the middle of the picture
    lcd_image_draw(&map_image, &tft,
        irow, icol,
        0, 0,
        TFT_WIDTH, TFT_HEIGHT);

        //lcd_image_draw(&map_image, &tft, 0, 0, 64, 64, 64, 64);
        //lcd_image_draw(&map_image, &tft, 32, 32, 64, 64, 64, 64);
    }

    int main() {
        setup();

        while (true) {
            scanJoystick();
            if (update == 1) {
                updateScreen();
            }
        }

        return 0;
    }


    // read the joystick and see if we should nudge the screen
    // change by 64 pixels each step
    void scanJoystick() {
        int vert = analogRead(JOY_VERT_ANALOG);
        int horiz = analogRead(JOY_HORIZ_ANALOG);
        int select = digitalRead(JOY_SEL);


        if (abs(horiz - JOY_CENTRE) > JOY_DEADZONE) {
            update = 1;
        }

        if (abs(vert - JOY_CENTRE) > JOY_DEADZONE) {
            update = 1;
        }
    }

    void updateScreen() {
        // display map here, using the irow and icol variables
        // for the part of the map to show

        update = 0;
    }
