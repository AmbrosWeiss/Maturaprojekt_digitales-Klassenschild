/*******************************************************************************************
*******************************************************************************************
**                                www.bastelgarage.ch                                    **
** Der Onlineshop mit Videoanleitungen und kompletten Bausätzen für Anfänger und Profis! **
*******************************************************************************************
*******************************************************************************************
** Hier zeige ich euch wie du einen BME280 ausliest und die gemessenen Werte auf einem   **
** e-Ink Display darstellst.                                                             **
**                                                                                       **
**                                                                                       **
** Verwendete Library:                                                                   **
** E-Ink Display Waveshare 200x200 1.54" GxEPD: https://github.com/ZinggJM/GxEPD         **
** BME280: https://github.com/e-radionicacom/BME280-Arduino-Library                      **
**                                                                                       **
** Autor: Philippe Keller                                                                **
** Datum: Oktober 2017                                                                   **
** Version: 1.0                                                                          **
*******************************************************************************************
  Pinbelegung:
  Display:      ESP32 Lolin32:
  BUSY          4
  RST           16
  DC            17
  CS            5   (SS)
  CLK           18  (SCK)
  DIN           23  (MOSI)
  GND           GND
  3.3V          3V

  BME280:       ESP32 Lolin32:
  VCC           3V
  GND           GND
  SCL           22
  SDA           21
*/

/************************( Importieren der genutzten Bibliotheken )************************/
#include <GxEPD.h>


// Genutztes Display 200x200 1.54inch Schwarz / Weiss E-Ink Display
// https://www.bastelgarage.ch/index.php?route=product/product&path=67_84&product_id=428
#include <GxGDEP015OC1/GxGDEP015OC1.cpp>
#include <Arduino.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
//#include <GxIO/GxIO.cpp>




/**************************(Definieren der genutzten Variabeln)****************************/
// Interval zum aktualisieren vom Display
int interval = 5000;
long lastTime = 5000;

GxIO_Class io(SPI, SS, 17, 16);  //SPI,SS,DC,RST
GxEPD_Class display(io, 16, 4);  //io,RST,BUSY

/*****************************************( Setup )****************************************/
void setup(void)
{
  Serial.begin(115200);

  display.init();                   // e-Ink Display initialisieren
  display.fillScreen(GxEPD_WHITE);  // Display Weiss füllen
  display.setRotation(1);           // Display um 90° drehen


  display.update();                 // Display aktualisieren
}

/*************************************(Hauptprogramm)**************************************/
void loop()
{



    display.setRotation(1);                // Display um 90° drehen
    display.setTextColor(GxEPD_BLACK);     // Schriftfarbe Schwarz
    display.print("Test");

    // Rechteck mit weissem Hintergrund erstellen
    //X-Position, Y-Position, Breite, Höhe, Farbe
    display.fillRect(0, 0, 200, 128, GxEPD_WHITE); //Xpos,Ypos,box-w,box-h



  }

}
