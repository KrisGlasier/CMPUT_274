#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define SD_CS 5
#define TFT_CS 6
#define TFT_DC 7
#define TFT_RST 8
#define JOY_SEL 9
#define JOY_VERT_ANALOG 0
#define JOY_HORIZ_ANALOG 1

#define TFT_WIDTH 128
#define TFT_HEIGHT 160

#define JOY_DEADZONE 64 // Only care about joystick movement if
                        // position is JOY_CENTRE +/- JOY_DEADZONE
#define JOY_CENTRE 512
#define JOY_SPEED 5

#define MILLIS_PER_FRAME 50 // 20fps

#define STEPS_PER_PIXEL 64


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

int g_joyX = TFT_WIDTH/2; // X-position of cursor in pixels
int g_joyY = TFT_HEIGHT/2; // Y-position of cursor in pixels
int g_cursorX = -1; // Drawn cursor position
int g_cursorY = -1;

uint16_t colour_state = ST7735_RED;

/**
  Scans the joystick for any movement. Updates g_joyX, g_joyY.
*/
void scanJoystick();

/**
  Refreshes the display. Draws the cursor at (g_joyX,g_joyY).
*/
void updateScreen();


int main() {
  init();
  Serial.begin(9600);
  // Init TFT
  tft.initR(INITR_BLACKTAB);
  // Init joystick
  pinMode(JOY_SEL, INPUT);
  digitalWrite(JOY_SEL, HIGH); // enables pull-up resistor
  Serial.println("initialized!");

  tft.fillScreen(ST7735_BLACK);

  int prevTime = millis();
  while (true) {
    scanJoystick();
    updateScreen();

    // Want: delay until prevTime + 50
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
    //int delta = horiz - JOY_CENTRE;

    // number of pixels we want to move, based on
    // the joystick position
    int delta = (horiz - JOY_CENTRE) / STEPS_PER_PIXEL;
    // move the cursor the given number of pixels
    g_joyX = constrain(g_joyX + delta, 0, TFT_WIDTH-1);

    // if (delta > 0) {
    //   g_joyX = constrain(g_joyX+JOY_SPEED, 0, TFT_WIDTH-1);
    // }
    // else {
    //   g_joyX = constrain(g_joyX-JOY_SPEED, 0, TFT_WIDTH-1);
    // }
  }
  // Update g_joyY
  if (abs(vert - JOY_CENTRE) > JOY_DEADZONE) {
    // number of pixels we want to move, based on
    // the joystick position
    int delta = (vert - JOY_CENTRE) / STEPS_PER_PIXEL;
    // move the cursor the given number of pixels
    g_joyY = constrain(g_joyY + delta, 0, TFT_HEIGHT-1);

    // int delta = vert - JOY_CENTRE;
    // if (delta > 0) {
    //   g_joyY = constrain(g_joyY+JOY_SPEED, 0, TFT_HEIGHT-1);
    // }
    // else {
    //   g_joyY = constrain(g_joyY-JOY_SPEED, 0, TFT_HEIGHT-1);
    // }
  }

  // Serial.print(vert); Serial.print(" ");
  // Serial.print(horiz); Serial.print(" ");
  // Serial.println(select);

  // Serial.print(g_joyX); Serial.print(" ");
  // Serial.println(g_joyY);
}


void updateScreen() {
  // tft.fillScreen(ST7735_BLACK);
  // tft.drawPixel(g_cursorX, g_cursorY, ST7735_BLACK);
  // tft.drawPixel(g_joyX, g_joyY, ST7735_WHITE);

  tft.drawLine(g_cursorX-5, g_cursorY, g_cursorX+5, g_cursorY, ST7735_BLACK);
  tft.drawLine(g_cursorX, g_cursorY-5, g_cursorX, g_cursorY+5, ST7735_BLACK);

  tft.drawLine(g_joyX-5, g_joyY, g_joyX+5, g_joyY, ST7735_WHITE);
  tft.drawLine(g_joyX, g_joyY-5, g_joyX, g_joyY+5, ST7735_WHITE);

  g_cursorX = g_joyX;
  g_cursorY = g_joyY;
}
