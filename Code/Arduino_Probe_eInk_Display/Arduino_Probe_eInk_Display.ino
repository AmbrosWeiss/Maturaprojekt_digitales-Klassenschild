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
#include <BME280.h>

// Genutztes Display 200x200 1.54inch Schwarz / Weiss E-Ink Display
// https://www.bastelgarage.ch/index.php?route=product/product&path=67_84&product_id=428
#include <GxGDEP015OC1/GxGDEP015OC1.cpp>


#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

//Bastelgarage Logo oder anderes Bild muss im selben Ordner sein
#include "IMG_0001.h"

// Genutzte Schrift importieren
// https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
#include <Fonts/FreeMonoBold18pt7b.h>

/**************************(Definieren der genutzten Variabeln)****************************/
// Interval zum aktualisieren vom Display
int interval = 5000;
long lastTime = 5000;


float temperature = 0;
float humidity = 0;
float pressure = 0;

BME280 bme;

GxIO_Class io(SPI, SS, 17, 16);  //SPI,SS,DC,RST
GxEPD_Class display(io, 16, 4);  //io,RST,BUSY

/*****************************************( Setup )****************************************/
void setup(void)
{
  Serial.begin(115200);

  //BME280 Einstellungen Details unter:
  //www.github.com/e-radionicacom/BME280-Arduino-Library/blob/master/examples/advancedSettings

  bme.settings(0, 3, 5, 4, 3, 3, 3);
  while (bme.begin()) {
    Serial.println("Es konnte kein BME289 gefunden werden, bitte Verdrahtung prüfen.");
    delay(1000);
  }


  display.init();                   // e-Ink Display initialisieren
  display.fillScreen(GxEPD_WHITE);  // Display Weiss füllen
  display.setRotation(1);           // Display um 90° drehen

  // Logo Bastelgarage auf Display darstellen
  // Bild,X-Position,Y-Position,Breite Bild, Höhe Bild, Farbe
  display.drawBitmap(gImage_IMG_0001, 0, (200 - 72), 200 , 72, GxEPD_BLACK);

  display.update();                 // Display aktualisieren
}

/*************************************(Hauptprogramm)**************************************/
void loop()
{

  if (millis() - lastTime > interval) {  // Prüfen ob Display aktualisiert werden muss
    lastTime = millis();

    // Lesen der Messwerte vom BME280
    temperature = bme.readTemp();
    pressure = (bme.readPressure() / 100.0F);
    humidity = bme.readHumidity();


    display.setRotation(1);                // Display um 90° drehen
    display.setTextColor(GxEPD_BLACK);     // Schriftfarbe Schwarz
    display.setFont(&FreeMonoBold18pt7b);  // Schrift definieren

    // Rechteck mit weissem Hintergrund erstellen
    //X-Position, Y-Position, Breite, Höhe, Farbe
    display.fillRect(0, 0, 200, 128, GxEPD_WHITE); //Xpos,Ypos,box-w,box-h


    // Temperatur schreiben
    display.setCursor(0, 35);
    if (temperature >= 20) display.print("+");
    if (temperature < 0 & temperature> -10) display.print("-");
    if (temperature >= 0 & temperature < 20) display.print(" +");
    display.print(temperature, 1);
    display.setCursor(120, 35);
    display.print(" C");

    // Da bei der Schrift kein Grad Zeichen vorhanden ist selber eins mit Kreisen erstellen
    display.fillCircle(130, 15, 5, GxEPD_BLACK);  //Xpos,Ypos,r,Farbe
    display.fillCircle(130, 15, 2, GxEPD_WHITE);  //Xpos,Ypos,r,Farbe

    // Luftfeuchtigkeit schreiben
    display.setCursor(0, 70);
    if (humidity >= 20) display.print(" ");
    if (humidity >= 0 & humidity < 20) display.print("  ");
    display.print(humidity, 1);
    display.print(" %");


    // Luftdruck schreiben
    display.setCursor(0, 105);
    display.print(pressure, 1);
    if (pressure < 1000) display.print(" ");
    display.print("hPa");

    // Teil refresh vom  Display
    display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);
  }

}
