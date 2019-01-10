//Tag der Offenen Tür
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
//#include <GxGDEW042Z15/GxGDEW042Z15.h>    // 4.2" b/w/r
//#include <GxGDEW0583T7/GxGDEW0583T7.h>    // 5.83" b/w
#include <GxGDEW075T8/GxGDEW075T8.h>      // 7.5" b/w
//#include <GxGDEW075Z09/GxGDEW075Z09.h>    // 7.5" b/w/r

#include "img/Rahmen1.h"
//#include GxEPD_BitmapExamples

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

GxIO_Class io(SPI, /*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16); // arbitrary selection of 17, 16
GxEPD_Class display(io, /*RST=*/ 16, /*BUSY=*/ 4); // arbitrary selection of (16), 4


void setup() {
  Serial.begin(115200);
  //Serial.println();
  //Serial.println("setup");

  display.init(115200); // enable diagnostic output on Serial

  //display.fillScreen(GxEPD_WHITE); 
  


  //Serial.println("setup done");

}

void loop() {

  display.drawBitmap(Rahmen1, sizeof(Rahmen1));
  delay(20000);
  display.update();

  /*
  //display.setRotation(1);                // Display um 90° drehen
  //display.setTextColor(GxEPD_RED);     // Schriftfarbe Grün
  //display.print("Test");
  //delay(2000);

  // Rechteck mit weissem Hintergrund erstellen
  //X-Position, Y-Position, Breite, Höhe, Farbe
  //display.fillRect(0, 0, 384, 640, GxEPD_BLACK); //Xpos,Ypos,box-w,box-h

  //display.update();


      
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
  delay(500000000000);
*/
}
