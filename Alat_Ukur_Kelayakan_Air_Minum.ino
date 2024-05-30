#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include "Colors.h"

#define MINPRESSURE 200
#define MAXPRESSURE 1000

// Touchscreen pins
const int XP = 7, XM = A1, YP = A2, YM = 6;
const int TS_LEFT = 169, TS_RT = 923, TS_BOT = 925, TS_TOP = 187;

// Konstanta untuk pemetaan koordinat
const int TFT_WIDTH = 320, TFT_HEIGHT = 240;

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

uint16_t DARKER_COLOR = 0x666666;

enum Screen {
              inisialisasiScreen,
              homeScreen,
              calibrationScreen,
              turbidityScreen,
              pHScreen,
              ujiScreen
            };
Screen currentScreen = homeScreen;

// Variabel untuk koordinat sentuhan
int pixel_x, pixel_y;

#define BUZZER_PIN 51

//fonts
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <FreeDefaultFonts.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include <Fonts/Opensans/8pt.h>
#include <Fonts/Opensans/9pt.h>
#include <Fonts/Opensans/10pt.h>
#include <Fonts/Opensans/11pt.h>
#include <Fonts/Opensans/12pt.h>
#include <Fonts/Opensans/13pt.h>
#include <Fonts/Opensans/13ptBold.h>
#include <Fonts/Opensans/14pt.h>
#include <Fonts/Opensans/14ptBold.h>
#include <Fonts/Opensans/15pt.h>
#include <Fonts/Opensans/15ptBold.h>


void setup() {
  Serial.begin(9600);
  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(1);
  tft.fillScreen(WHITE);

  pinMode(BUZZER_PIN, OUTPUT); // Set pin buzzer sebagai output
  

  screenInisial();

}

void loop() {
  
  TSPoint p = ts.getPoint();
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);

  if (pressed) {
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    //pixel_x = map(p.y, TS_TOP, TS_BOT, 0, TFT_WIDTH);
    //pixel_y = map(p.x, TS_LEFT, TS_RT, 0, TFT_HEIGHT);

    pixel_x = map(p.y, TS_LEFT, TS_RT, 0, TFT_WIDTH);
    pixel_y = map(p.x, TS_TOP, TS_BOT, 0, TFT_HEIGHT);

    Serial.print("Touch detected: ");
    Serial.print("X = ");
    Serial.print(pixel_x);
    Serial.print(", Y = ");
    Serial.println(pixel_y);

    switch (currentScreen) {
      case homeScreen:
        if (pixel_x > 30 && pixel_x < 140 && pixel_y > 100 && pixel_y < 160) {
          Serial.println("Switching to Calibration Screen");
          drawButtonPressed(30, 100, 110, 60, 15, BLACK);
          delay(150);
          drawButton(30, 100, 110, 60, 15, RED, "SELF", "CALIBRATE");
          screenCalibration();
          currentScreen = calibrationScreen;
        } else if (pixel_x > 180 && pixel_x < 290 && pixel_y > 100 && pixel_y < 160) {
          Serial.println("Switching to Sample Test Screen");
          drawButtonPressed(180, 100, 110, 60, 15, BLACK);
          delay(150);
          drawButton(180, 100, 110, 60, 15, GREEN, "SAMPLE", "TEST");
          screenUji();
          currentScreen = ujiScreen;
        }
        break;
      case calibrationScreen:
        if (pixel_x > 130 && pixel_x < 190 && pixel_y > 205 && pixel_y < 220) {
          Serial.println("Returning to Home Screen from Calibration Screen");
          digitalWrite(BUZZER_PIN, HIGH); // Nyalakan buzzer
          drawButtonPressed(130, 205, 60, 25, 15, BLACK);
          delay(50);
          drawButton(130, 205, 60, 25, 15, BLUE, "BACK", NULL);
          digitalWrite(BUZZER_PIN, LOW);  // Matikan buzzer
          screenHome();
          currentScreen  = homeScreen;
        } else if(pixel_x > 30 && pixel_x < 140 && pixel_y > 100 && pixel_y < 160){
          drawButtonPressed(30, 100, 110, 60, 15, BLACK);
          delay(150);
          drawButton(30, 100, 110, 60, 15, RED, "TURBIDITY", "SENSOR");
          screenTurbidity();
          currentScreen  = turbidityScreen;
        }  else if (pixel_x > 180 && pixel_x < 290 && pixel_y > 100 && pixel_y < 160) {
          Serial.println("Switching to Sample Test Screen");
          drawButtonPressed(180, 100, 110, 60, 15, BLACK);
          delay(150);
          drawButton(180, 100, 110, 60, 15, GREEN, "pH", "SENSOR");
          screenPH();
          currentScreen = pHScreen;
        }
        break;
      case turbidityScreen:
        if (pixel_x > 130 && pixel_x < 190 && pixel_y > 205 && pixel_y < 220) {
          Serial.println("Returning to Home Screen from Calibration Screen");
          drawButtonPressed(130, 205, 60, 25, 15, BLACK);
          delay(150);
          drawButton(130, 205, 60, 25, 15, BLUE, "BACK", NULL);
          screenCalibration();
          currentScreen  = calibrationScreen;
        }
      break;
      case pHScreen:
        if (pixel_x > 130 && pixel_x < 190 && pixel_y > 205 && pixel_y < 220) {
          Serial.println("Returning to Home Screen from Calibration Screen");
          drawButtonPressed(130, 205, 60, 25, 15, BLACK);
          delay(150);
          drawButton(130, 205, 60, 25, 15, BLUE, "BACK", NULL);
          screenCalibration();
          currentScreen  = calibrationScreen;
        }
      break;
      case ujiScreen:
        if (pixel_x > 130 && pixel_x < 190 && pixel_y > 205 && pixel_y < 220) {
          Serial.println("Returning to Home Screen from Calibration Screen");
          drawButtonPressed(130, 205, 60, 25, 15, BLACK);
          delay(150);
          drawButton(130, 205, 60, 25, 15, BLUE, "BACK", NULL);
          screenHome();
          currentScreen  = homeScreen;
        }
      break;
    }

    // Cetak nilai currentScreen untuk debugging
    Serial.print("Current screen: ");
    Serial.println(currentScreen);
  }

  delay(100);



}




//
void screenInisial(){
  //tampilan battery
  battery();

  tft.setFont(&Open_Sans_Bold_15);
  tft.setCursor(65, 50);
  tft.setTextColor(RED);
  tft.setTextSize(1);
  tft.print("MODUL TUGAS AKHIR");

  tft.setFont(&Open_Sans_Bold_15);
  tft.setCursor(12, 70);
  tft.setTextColor(RED);
  tft.setTextSize(1);
  tft.print("ALAT UKUR KELAYAKAN AIR MINUM");

  tft.setFont(&Open_Sans_Bold_13);
  tft.setCursor(70, 100);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("NAMA: ZAVILAYETA");
  
  tft.setFont(&Open_Sans_Bold_13);
  tft.setCursor(70, 120);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("NIM    : P220301200");

  tft.setFont(&Open_Sans_Regular_13);
  tft.setCursor(16, 145);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("PEMBIMBING 1: ABCDEFG HIJKLMN OPQRS");

  tft.setFont(&Open_Sans_Regular_13);
  tft.setCursor(16, 165);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("PEMBIMBING 2: ABCDEFG HIJKLMN OPQRS");

  for(int i = 0; i <=225; i+=25){
  uint16_t color = random(0xF81F); // Generate random color
   tft.drawCircle(45 + i, 210, 10, color);
   delay(150); 
  }

  screenHome();
}

//Tampilan utama
void screenHome(){
  tft.fillScreen(WHITE);

  battery();

  tft.setFont(&Open_Sans_Bold_14);
  tft.setCursor(15, 50);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("ALAT UKUR KELAYAKAN AIR MINUM");

  tft.fillRect(0, 70, 320, 3, DARKGREY);

  drawButton(30, 100, 110, 60, 15, RED, "SELF", "CALIBRATE");
  drawButton(180, 100, 110, 60, 15, GREEN, "SAMPLE", "TEST");
  
}

//Tampilan menu calibration
void screenCalibration(){
  tft.fillScreen(WHITE);

  battery();

  tft.setFont(&Open_Sans_Bold_15);
  tft.setCursor(80, 50);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("SELF CALIBRATION");

  tft.fillRect(0, 70, 320, 3, DARKGREY);

  drawButton(30, 100, 110, 60, 15, RED, "TURBIDITY", "SENSOR");
  drawButton(180, 100, 110, 60, 15, GREEN, "pH", "SENSOR");

  tft.fillRect(0, 192, 320, 3, DARKGREY);

  drawButton(130, 205, 60, 25, 15, BLUE, "BACK", NULL);
}

//Tampilan Calibration Turbidity
void screenTurbidity(){
  tft.fillScreen(WHITE);

  battery();

  tft.setFont(&Open_Sans_Bold_15);
  tft.setCursor(89, 50);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("SELF CALIBRATION");

  tft.fillRect(0, 70, 320, 3, DARKGREY);

  tft.setFont(&Open_Sans_Bold_15);
  tft.setCursor(85, 98);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("TURBIDITY SENSOR");

  tft.setFont(&FreeSevenSegNumFont);
  tft.setCursor(125, 165);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("99");

  tft.fillRect(0, 192, 320, 3, DARKGREY);

  drawButton(130, 205, 60, 25, 15, BLUE, "BACK", NULL);
}

//
void screenPH(){
  tft.fillScreen(WHITE);

  battery();

  tft.setFont(&Open_Sans_Bold_15);
  tft.setCursor(89, 50);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("SELF CALIBRATION");

  tft.fillRect(0, 70, 320, 3, DARKGREY);

  tft.setFont(&Open_Sans_Bold_15);
  tft.setCursor(115, 98);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("pH SENSOR");

  tft.setFont(&FreeSevenSegNumFont);
  tft.setCursor(125, 165);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("99");

  tft.fillRect(0, 192, 320, 3, DARKGREY);

  drawButton(130, 205, 60, 25, 15, BLUE, "BACK", NULL);
}

//
void screenUji(){
  tft.fillScreen(WHITE);

  battery();

  tft.setFont(&Open_Sans_Bold_15);
  tft.setCursor(80, 50);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("PENGUJIAN SAMPEL");

  tft.fillRect(0, 70, 320, 3, DARKGREY);

  tft.fillRect(159, 70, 2, 82, DARKGREY);//GARIS Y

  tft.setFont(&Open_Sans_Regular_9);
  tft.setCursor(30, 90);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("KEKERUHAN (NTU)");

  tft.setFont(&Open_Sans_Regular_9);
  tft.setCursor(50, 135);
  tft.setTextColor(BLACK);
  tft.setTextSize(4);
  tft.print("00");

  tft.setFont(&Open_Sans_Regular_9);
  tft.setCursor(180, 90);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("DERAJAT KEASAMAN (pH)");

  tft.setFont(&Open_Sans_Regular_9);
  tft.setCursor(215, 135);
  tft.setTextColor(BLACK);
  tft.setTextSize(4);
  tft.print("00");


  tft.fillRect(0, 152, 320, 3, DARKGREY);

  tft.setFont(&Open_Sans_Bold_15);
  tft.setCursor(80, 180);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.print("LAYAK KONSUMSI");

  tft.fillRect(0, 192, 320, 3, DARKGREY);

  drawButton(130, 205, 60, 25, 15, BLUE, "BACK", NULL);
}

void battery(){

  tft.fillRect(0, 0, 320, 14, BLACK);

  tft.setFont(&Open_Sans_Regular_10);//tinggi 12 px
  tft.setCursor(220, 10);
  tft.setTextColor(GREEN);
  tft.setTextSize(0);
  tft.print("Battery : 100 %");
}

void drawButton(int x, int y, int w, int h, int radius, uint16_t fillColor, const char* text1, const char* text2) {
  tft.fillRoundRect(x, y, w, h, radius, fillColor);
  tft.drawRoundRect(x, y, w, h, radius, BLACK);
  tft.setFont(&Open_Sans_Bold_15);
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  
  int16_t x1, y1;
  uint16_t w1, h1;
  
  // Center text1
  tft.getTextBounds(text1, 0, 0, &x1, &y1, &w1, &h1);
  tft.setCursor(x + (w - w1) / 2, 10 + y + (h / 4));
  tft.print(text1);
  
  // Center text2
  tft.getTextBounds(text2, 0, 0, &x1, &y1, &w1, &h1);
  tft.setCursor(x + (w - w1) / 2, y + (h / 2) + (h / 4));
  tft.print(text2);
}

void drawButtonPressed(int x, int y, int w, int h, int radius, uint16_t fillColor) {
  // Draw a slightly darker button background for pressed effect
  tft.fillRoundRect(x, y, w, h, radius, fillColor);

  // Optional: Draw a thin white highlight around the button
  tft.drawRoundRect(x - 1, y - 1, w + 2, h + 2, radius, WHITE);

  // Draw inner black border on top
  tft.drawRoundRect(x, y, w, h, radius, BLACK);
}

