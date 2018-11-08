// GxEPD_Example : test example for e-Paper displays from Waveshare and from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display,
// available on http://www.good-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// The e-paper displays are available from:
//
// https://www.aliexpress.com/store/product/Wholesale-1-54inch-E-Ink-display-module-with-embedded-controller-200x200-Communicate-via-SPI-interface-Supports/216233_32824535312.html
//
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_8363&product_id=35120
// or https://www.aliexpress.com/store/product/E001-1-54-inch-partial-refresh-Small-size-dot-matrix-e-paper-display/600281_32815089163.html
//

// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Dispay ePaper for Arduino : https://forum.arduino.cc/index.php?topic=436411.0

// mapping suggestion from Waveshare SPI e-Paper to Wemos D1 mini
// BUSY -> D2, RST -> D4, DC -> D3, CS -> D8, CLK -> D5, DIN -> D7, GND -> GND, 3.3V -> 3.3V

// mapping suggestion from Waveshare SPI e-Paper to generic ESP8266
// BUSY -> GPIO4, RST -> GPIO2, DC -> GPIO0, CS -> GPIO15, CLK -> GPIO14, DIN -> GPIO13, GND -> GND, 3.3V -> 3.3V

// mapping suggestion for ESP32, e.g. LOLIN32, see .../variants/.../pins_arduino.h for your board
// NOTE: there are variants with different pins for SPI ! CHECK SPI PINS OF YOUR BOARD
// BUSY -> 4, RST -> 16, DC -> 17, CS -> SS(5), CLK -> SCK(18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V

// new mapping suggestion for STM32F1, e.g. STM32F103C8T6 "BluePill"
// BUSY -> A1, RST -> A2, DC -> A3, CS-> A4, CLK -> A5, DIN -> A7

// mapping suggestion for AVR, UNO, NANO etc.<
// BUSY -> 7, RST -> 9, DC -> 8, CS-> 10, CLK -> 13, DIN -> 11

// include library, include base class, make path known
#include <GxEPD.h>

// select the display class to use, only one
//#include <GxGDEP015OC1/GxGDEP015OC1.h>    // 1.54" b/w
//#include <GxGDEW0154Z04/GxGDEW0154Z04.h>  // 1.54" b/w/r 200x200
//#include <GxGDEW0154Z17/GxGDEW0154Z17.h>  // 1.54" b/w/r 152x152
//#include <GxGDE0213B1/GxGDE0213B1.h>      // 2.13" b/w
//#include <GxGDEW0213Z16/GxGDEW0213Z16.h>  // 2.13" b/w/r
//#include <GxGDEH029A1/GxGDEH029A1.h>      // 2.9" b/w
//#include <GxGDEW029Z10/GxGDEW029Z10.h>    // 2.9" b/w/r
//#include <GxGDEW027C44/GxGDEW027C44.h>    // 2.7" b/w/r
//#include <GxGDEW027W3/GxGDEW027W3.h>      // 2.7" b/w
//#include <GxGDEW042T2/GxGDEW042T2.h>      // 4.2" b/w
#include <GxGDEW042Z15/GxGDEW042Z15.h>    // 4.2" b/w/r
//#include <GxGDEW0583T7/GxGDEW0583T7.h>    // 5.83" b/w
//#include <GxGDEW075T8/GxGDEW075T8.h>      // 7.5" b/w
//#include <GxGDEW075Z09/GxGDEW075Z09.h>    // 7.5" b/w/r

#include GxEPD_BitmapExamples

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>


#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>


// for SPI pin definitions see e.g.:
// C:\Users\xxx\Documents\Arduino\hardware\espressif\esp32\variants\lolin32\pins_arduino.h

GxIO_Class io(SPI, /*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16); // arbitrary selection of 17, 16
GxEPD_Class display(io, /*RST=*/ 16, /*BUSY=*/ 4); // arbitrary selection of (16), 4


void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");

  display.init(115200); // enable diagnostic output on Serial
  display.fillScreen(GxEPD_WHITE); 
  display.update();

  Serial.println("setup done");
}

void loop()
{
  //display.println("Test!");
  //display.update();

  display.setRotation(1);                // Display um 90° drehen
    display.setTextColor(GxEPD_BLACK);     // Schriftfarbe Schwar
    display.print("Test");
    delay(20000);

    // Rechteck mit weissem Hintergrund erstellen
    //X-Position, Y-Position, Breite, Höhe, Farbe
    display.fillRect(0, 0, 384, 640, GxEPD_BLACK); //Xpos,Ypos,box-w,box-h

    display.update();
  
  /*void showFont(const char name[], const GFXfont * f)
  {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(f);
    display.setCursor(0, 0);
    display.println();
    display.println(name);
    display.println(" !\"#$%&'()*+,-./");
    display.println("0123456789:;<=>?");
    display.println("@ABCDEFGHIJKLMNO");
    display.println("PQRSTUVWXYZ[\\]^_");
#if defined(HAS_RED_COLOR)
    display.setTextColor(GxEPD_RED);
#endif
    display.println("`abcdefghijklmno");
    display.println("pqrstuvwxyz{|}~ ");
    display.update();
    delay(5000);
  }*/

  /*void showFontCallback()
  {
    const char* name = "FreeMonoBold9pt7b";
    const GFXfont* f = &FreeMonoBold9pt7b;
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(f);
    display.setCursor(0, 0);
    display.println();
    display.println(name);
    display.println(" !\"#$%&'()*+,-./");
    display.println("0123456789:;<=>?");
    display.println("@ABCDEFGHIJKLMNO");
    display.println("PQRSTUVWXYZ[\\]^_");
#if defined(HAS_RED_COLOR)
    display.setTextColor(GxEPD_RED);
#endif
    display.println("`abcdefghijklmno");
    display.println("pqrstuvwxyz{|}~ ");
  }*/

  void drawCornerTest()
    {
    display.drawCornerTest();
    delay(5000);
    uint8_t rotation = display.getRotation();
    for (uint16_t r = 0; r < 4; r++)
    {
      display.setRotation(r);
      display.fillScreen(GxEPD_WHITE); //Xpos,Ypos,box-w,box-h
      
      display.fillRect(0, 0, 370, 148, GxEPD_BLACK); //Xpos,Ypos,box-w,box-h //Feld_Raum
      display.fillRect(4, 4, 366, 265, GxEPD_WHITE); //Xpos,Ypos,box-w,box-h //Rahmen-Feld_Raum (Rahmenstärke: 4 Pixel)

      display.fillRect(0, 148, 409, 236, GxEPD_BLACK); //Xpos,Ypos,box-w,box-h //Feld_Klasse
      display.fillRect(4, 144, 405, 232, GxEPD_WHITE); //Xpos,Ypos,box-w,box-h //Rahmen-Feld_Klasse (Rahmenstärke: 4 Pixel)

      display.fillRect(409, 266, 231, 118, GxEPD_BLACK); //Xpos,Ypos,box-w,box-h //Feld_Datum&Einheit
      display.fillRect(413, 270, 401, 228, GxEPD_WHITE); //Xpos,Ypos,box-w,box-h //Rahmen-Feld_Datum&Einheit (Rahmenstärke: 4 Pixel)

      display.fillRect(409, 148, 231, 118, GxEPD_BLACK); //Xpos,Ypos,box-w,box-h //Feld_Lehrer
      display.fillRect(413, 152, 397, 223, GxEPD_WHITE); //Xpos,Ypos,box-w,box-h //Rahmen-Feld_Lehrer (Rahmenstärke: 4 Pixel)

      display.fillRect(409, 148, 231, 118, GxEPD_BLACK); //Xpos,Ypos,box-w,box-h //Feld_Lehrer
      display.fillRect(413, 152, 397, 223, GxEPD_WHITE); //Xpos,Ypos,box-w,box-h //Rahmen-Feld_Lehrer (Rahmenstärke: 4 Pixel)

      
      
      display.update();
      delay(5000);
    }
    display.setRotation(rotation); // restore
    }

  
}
