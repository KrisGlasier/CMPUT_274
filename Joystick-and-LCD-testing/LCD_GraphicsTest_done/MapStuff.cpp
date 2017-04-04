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
#define JOY_DEADZONE 64
#define JOY_CENTRE 512
#define JOY_SPEED 5
#define JOY_SEL 9

#define MILLIS_PER_FRAME 50 // 20fps
#define STEPS_PER_PIXEL 64

#define TFT_WIDTH 128
#define TFT_HEIGHT 160

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

lcd_image_t map_image = { "yeg-big.lcd", 2048, 2048 };

uint16_t Mcol = 2048, Mrow = 2048;
uint16_t icol = Mrow/2;
uint16_t irow = Mrow/2;

int g_joyX = TFT_WIDTH/2;   // X-position of cursor in pixels
int g_joyY = TFT_HEIGHT/2;  // Y-position of cursor in pixels
int g_cursorX = 64;         // Drawn cursor position
int g_cursorY = 80;
int CursorAtEdge = 0;

void scanJoystick();
void updateScreen();
void RedrawMap();

void setup(void) {
    init();

    Serial.begin(9600);

    tft.initR(INITR_BLACKTAB);   // initialize a ST7735R chip, black tab

    pinMode(JOY_SEL, INPUT);
    digitalWrite(JOY_SEL, HIGH); // enables pull-up resistor

    // Send initialization message to Serial Monitor for confirmation
    Serial.print("Initializing SD card...");
    if (!SD.begin(SD_CS)) {
        Serial.println("failed!");
        return;
    }
    Serial.println("OK!");

    // fill the whole screen with a part of the picture of Edmonton
    lcd_image_draw(&map_image, &tft,
        icol, irow,
        0, 0,
        TFT_WIDTH, TFT_HEIGHT);

}

int main() {
    setup();
    int prevTime = millis();

    // Continually scan the joystick. If any changed update screen accordingly
    while (true) {
      scanJoystick();
      if(g_joyX != g_cursorX || g_joyY != g_cursorY){
        updateScreen();
      }

      // Time delay- initially set to 20 FPS, or 50 milliseconds/frame
      int t = millis();
      if (t - prevTime < MILLIS_PER_FRAME) {
        delay(MILLIS_PER_FRAME - (t - prevTime));
      }
      prevTime = millis();
    }

    Serial.end();
    return 0;
}

void scanJoystick() {
  int vert = analogRead(JOY_VERT_ANALOG);
  int horiz = analogRead(JOY_HORIZ_ANALOG);
  int select = digitalRead(JOY_SEL);

  // Update g_joyX
  if (abs(horiz - JOY_CENTRE) > JOY_DEADZONE) {
    //The farther the joystick is moved, the faster the cursor moves.
    int delta = (horiz - JOY_CENTRE) / STEPS_PER_PIXEL;
    // move the cursor the given number of pixels
    g_joyX = constrain(g_joyX + delta, 0, TFT_WIDTH-1);
  }

  // Update g_joyY
  if (abs(vert - JOY_CENTRE) > JOY_DEADZONE) {
    int delta = (vert - JOY_CENTRE) / STEPS_PER_PIXEL;
    g_joyY = constrain(g_joyY + delta, 0, TFT_HEIGHT-1);
  }

}

void updateScreen() {
    // Draw the map over the old cursor and make the new cursor again.
    lcd_image_draw(&map_image, &tft,
                    icol + g_cursorX-3, irow + g_cursorY-3,
                    g_cursorX-3, g_cursorY-3,
                    7, 7);

    tft.drawLine(g_joyX-3, g_joyY, g_joyX+3, g_joyY, ST7735_RED);
    tft.drawLine(g_joyX, g_joyY-3, g_joyX, g_joyY+3, ST7735_RED);


    // ELSE-IF statements for when the cursor hits the end of the screen.
    /*
    Basic structure for the following set of nested if statements is...
    if ( check what bounary we're on ){
        if( We're farther than half a screen width away from boarder){
            Move screen -> redraw cursor -> Change cursor state
        }
        else if( Check if we're within a screen length away
                from boarder AND cursor isn't touching boarder){
            Draw screen -> ChangeCursor state
        }
        else{ do nothing }
    }
    */
    if     (g_cursorX  >  TFT_WIDTH-2){
        if(icol <= (Mcol - 3*TFT_WIDTH/2) ){
            icol += 64;
            RedrawMap();
            g_joyX = 64;
            g_joyY = 80;
            CursorAtEdge = 0;
        }
        else if( (icol > (Mcol - 3*TFT_WIDTH/2)) && (CursorAtEdge = 0) ){
            icol = Mcol - TFT_WIDTH;
            RedrawMap();
            CursorAtEdge = 1;
            }
        else { ; }
    }
    else if(g_cursorX  <  1){
        if(icol >= 64){
            icol -= 64;
            RedrawMap();
            g_joyX = 64;
            g_joyY = 80;
            CursorAtEdge = 0;
        }
        else if( (icol  < 64) && (CursorAtEdge = 0) ){
            icol = 0;
            RedrawMap();
            CursorAtEdge = 1;
            }
        else { ; }
    }
    else if(g_cursorY  >  TFT_HEIGHT-2){
        if(irow < (Mrow - TFT_HEIGHT)){
            irow += 64;
            RedrawMap();
            g_joyX = 64;
            g_joyY = 159-64;
            CursorAtEdge = 0;
            }
        else if ( (irow >= (Mrow - 32 - 3*TFT_WIDTH/2 )) && (CursorAtEdge = 0) ){
            irow = Mrow - TFT_HEIGHT;
            RedrawMap();
            CursorAtEdge = 1;
        }
        else { ; }
    }
    else if(g_cursorY  <  1){
        if(irow >= 64){
            irow -= 64;
            RedrawMap();
            g_joyX = 64;
            g_joyY = 80;
            CursorAtEdge = 0;
            }
        else if ( (irow < 64) && (CursorAtEdge = 0) ){
            irow = 0;
            RedrawMap();
            CursorAtEdge = 1;
        }
        else { ; }
    }
    g_cursorX = g_joyX;
    g_cursorY = g_joyY;
}

void RedrawMap(){
    //Used several times and looks nicer broken up. Saves lines.
    lcd_image_draw(&map_image, &tft,
                    icol, irow,
                    0, 0,
                    TFT_WIDTH, TFT_HEIGHT);
}
