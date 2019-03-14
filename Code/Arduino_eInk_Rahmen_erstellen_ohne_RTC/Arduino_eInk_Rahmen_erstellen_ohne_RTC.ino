//reguläres Programm
#include <GxEPD.h>
//#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

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
//#include <GxGDEW042Z15/GxGDEW042Z15.h>    // 4.2" b/w/r
//#include <GxGDEW0583T7/GxGDEW0583T7.h>    // 5.83" b/w
//#include <GxGDEW075T8/GxGDEW075T8.h>      // 7.5" b/w
#include <GxGDEW075Z09/GxGDEW075Z09.h>    // 7.5" b/w/r

#include "img/Rahmen13.h"
//#include GxEPD_BitmapExamples

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>



GxIO_Class io(SPI, /*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16); // arbitrary selection of 17, 16
GxEPD_Class display(io, /*RST=*/ 16, /*BUSY=*/ 4); // arbitrary selection of (16), 4





void setup() {
  Serial.begin(115200);
  display.init(115200); // enable diagnostic output on Serial
  //Serial.println("setup done");

}



void loop() {


  //display.drawBitmap(Rahmen13, sizeof(Rahmen13));
  display.BitmapToBuffer(Rahmen13, sizeof(Rahmen13));
  display.setTextSize(4);
  display.setTextColor(GxEPD_BLACK, GxEPD_WHITE);
  
  //aktuelle Klasse
  display.setCursor(220,108);
  display.print("5 BHELS");
  
  //Raumnummer
  display.setTextSize(4);
  display.setCursor(220,295);
  display.print("318");
  
  //Stammklasse
  display.setTextSize(2);
  display.setCursor(220,350);
  display.print("5 BHELS");
  
  //Datum
  display.setTextSize(2);
  //x,y
  display.setCursor(540,22);
  display.print("07.03");
  
  //Einheit
  display.setTextSize(2);
  display.setCursor(540,45);
  display.print("4. EH");
  
  //Lehrer
  display.setTextSize(2);
  display.setCursor(540,108);
  display.print("ENGEL");
  
  //Gegenstand
  display.setTextSize(2);
  display.setCursor(570,187);
  display.print("D");
  
  //nächster Lehrer
  display.setTextSize(2);
  display.setCursor(500,285);
  display.print("WOLF");
  
  //nächste Klasse
  display.setTextSize(2);
  display.setCursor(500,305);
  display.print("5 BHELS");
  
  //nächstes Fach
  display.setTextSize(2);
  display.setCursor(500,327);
  display.print("HWE");

  
  display.update();
  delay(3600000);



}
