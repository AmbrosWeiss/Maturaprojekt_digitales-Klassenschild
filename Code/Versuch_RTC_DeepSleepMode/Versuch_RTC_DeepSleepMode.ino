//reguläres Programm
#include "GxEPD.h"
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
//eigentlich das Richtige, aber da ich eine eigene Funktion geschrieben habe, benutze ich diese lokal
//#include <GxGDEW075Z09/GxGDEW075Z09.h>    // 7.5" b/w/r

#include "GxEPD-master-modifiziert/src/GxGDEW075Z09/GxGDEW075Z09_modifiziert.h"    // 7.5" b/w/r

//Einfügen der Bitmap
#include "img/Rahmen13.h"

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

//____________________RTC__________________
#include <WiFi.h>
#include "NTPClientSeparated/NTPClientSeparated.h"
#include <WiFiUdp.h>

//damit Uhrzeit/Datum in epoch code umwandeln
#include "Time-master/Time.h"

//für Wochentag
char daysOfTheWeek[7] = {'So', 'Mo', 'Di', 'Mi', 'Do', 'Fr', 'Sa'};

//Netzwerk (WLAN) Daten
const char* ssid = "HUAWEI P20 lite";
const char* password = "ambros123";

//NTP-Client definieren um Zeit zu bekommen
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

//Variablem um Uhrzeit und Datum zu speichern
String formattedDate;
String dayStamp;
String timeStamp;

//___________________RTC_ENDE______________




//___________________DEEP_SLEEP____________

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in seconds) */
RTC_DATA_ATTR int bootCount = 0;

//___________________DEEP_SLEEP_ENDE______________




//___________________AUFWACHZEITEN_UNTER_DER_WOCHE___________________________________________________________
const char wakeuptimeWeekdays[18] = {0, 0, 0, 0, 0, 0, 55, 51, 46, 41, 46, 41, 36, 26, 21, 16, 06, 0};
//           Uhrzeit in Stunden:     0  1  2  3  4  5   6   7   8   9  10  11  12  13  14  15  16  17
//____________________________________________________________________________________________________________

GxIO_Class io(SPI, /*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16); // arbitrary selection of 17, 16
GxEPD_Class display(io, /*RST=*/ 16, /*BUSY=*/ 4); // arbitrary selection of (16), 4





void setup() {
  Serial.begin(115200);
  display.init(115200); // enable diagnostic output on Serial
  //Serial.println("setup done");


  //_____________________RTC____________________________

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  
  //start web server
  WiFi.localIP();

  //initialisiere NTPClient um Zeit zu bekommen
  timeClient.begin();

  //Offsett Zeit in Sekunden für die Richtige Zeitzone einistellen
  //GMT +1 = 3600
  timeClient.setTimeOffset(3600);
//_______________________RTC_ENDE_________________________
}



void loop() {

//_____________________RTC________________________________

//manchmal ruft der NTPClient 1970 auf, um sicherzustellen --> nicht passiert --> Update erzwingen
  while(!timeClient.update()){
    timeClient.forceUpdate();
  }

  //Das formatierte Datum ("formattedDate") kommt in folgendem Format an:
  //2019-01-10T11:11:40Z
  formattedDate = timeClient.getFormattedDate();

  //Um Datum und Uhrzeit extra zu bekommen, müssen wir diese extrahieren
  //Datum extrahieren
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);

  //Uhrzeit extrahieren
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  delay(1000);
  //_______________________RTC_ENDE_________________________________



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
  display.setTextSize(1);
  //x,y
  display.setCursor(540,22);
  display.print(timeStamp);

  //aktueller Wochentag
  display.print(daysOfTheWeek[timeClient.getDay()]);
  
  //Einheit
  display.setTextSize(2);
  display.setCursor(540,45);
  display.print("9. EH");
  
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
  delay(60);

/*
String siebzehnUhr = "17:00:00";
//wenn es Freitag um 17:00 (61.200 Sekunden) ist, dann soll der ESP schlafen und erst
//am Montag um 07:00 wieder aufwachen
if(daysOfTheWeek[now.dayOfTheWeek()] == daysOfTheWeek[5] && timeStamp >= siebzehnUhr)
{
  //223200 ist die Zeit von Freitag von 17:00 bis Montag um 07:00
  esp_sleep_enable_timer_wakeup(223200 * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
  //delay(3600000);
}

Probeprogramm
String neunUhr = "09:35:00";
if(daysOfTheWeek[now.dayOfTheWeek()] == daysOfTheWeek[4] && timeStamp >= neunUhr)
{
  display.setCursor(220,108);
  display.print("Good Night");
  
  //Zeit von 4 Minuten
  esp_sleep_enable_timer_wakeup(240 * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
  display.setTextSize(4);
  display.print("I sincerely hope that i never ever see this sentence on the display.");
  //delay(3600000);
}*/

  int schlafdauer = getSleepingTime();
  esp_sleep_enable_timer_wakeup(schlafdauer);
  esp_deep_sleep_start();
}


int getSleepingTime()
{
  //ruft die aktuelle Stunde auf
  int aktuelleStunde = timeClient.getHours();
  //ruft die aktuelle Minute auf
  int aktuelleMinute = timeClient.getMinutes();
  //ruft den aktuellen Wochentag (Montag, ...)auf
  int aktuellerWochentag = timeClient.getDay();


  //Wenn es Mo-Fr ist --> aktualiesierungszyklen nach dem Array wakeuptimeWeekdays
  if(aktuellerWochentag > 0 && aktuellerWochentag <=5)
  {
      //wenn die vorgegebene Aufwachzeit in der aktuellen Stunde (z.B. Stunde 10) im Array ander Stelle (9+1) größer ist,
      //als die aktuelle Zeit so wird die Vorgegebene minus der aktuellen Zeit gerechnet 
      //Beispiel: vorg. 10:46 / akt. 10:30 --> 10:46 > 10:30 --> 46-30 = 16 --> 16 * 60 --> 960 Sekunden schlafen legen
      if(wakeuptimeWeekdays[aktuelleStunde + 1] >= aktuelleMinute)
      {
        int schlafdauer = wakeuptimeWeekdays[aktuelleStunde+1] - aktuelleMinute;
        int schlafdauerSek = schlafdauer * 60;
        return schlafdauerSek;
      }
      //vorgegebene Zeit kleiner als akt. Zeit, Stunde aber eine anderere (Bsp: vorg. 10:46 / akt. = 10:50) --> stunde wird erhöht, weil er somit erst in der nächsten Stunde
      //in diesem Beispiel um 11:41 aufwachen muss --> die Werte werden in echo-Code umgewandelt und wieder die vorgegebene - aktuelle
      if(wakeuptimeWeekdays[aktuelleStunde+1] <= aktuelleMinute)
      {
        //Uhrzeit und Datum in echo-Code
        time_t aktuelleUhrzeit = tmConvert_t(timeClient.getYear(), timeClient.getMonth(), timeClient.getDayNumber(), timeClient.getHours(),timeClient.getMinutes(), timeClient.getSeconds());
        int aktStd = timeClient.getHours();

        //gibt an, wann er wieder aufwachen muss (gilt nur für unter der Woche)          
        //"aktStd+1", weil erst in der nächsten Stunde wieder aufwachen (oben angegeben)
        //"wakeuptimeWeekdays[aktuelleStunde+1]", so erhält man die Minute, wann er in der nächsten Stunde aufwachen muss
        time_t aufwachzeit = tmConvert_t(timeClient.getYear(), timeClient.getMonth(), timeClient.getDayNumber(), aktStd+1, wakeuptimeWeekdays[aktuelleStunde+1], timeClient.getSeconds());

        //wie lange er schlafen darf
        //Wert sollte schon in Sekunden sein --> weil echo sekunden zurück gibt
        int schlafdauer = aufwachzeit - aktuelleUhrzeit;
        return schlafdauer;
      }
  }

  else
  {
    //wenn Samstag, SOnntag, Freitag nach 17:00 oder Ferien --> wieder so lange schlafen legen, bis er aufwachen darf
  }
}


//Funktion, um ein Datum bzw Uhrzeit in einen epoch Code zu wandeln
time_t tmConvert_t(int YYYY, byte MM, byte DD, byte hh, byte mm, byte ss)
{
 
  tmElements_t tmSet;
  tmSet.Year = YYYY - 1970;
  tmSet.Month = MM;
  tmSet.Day = DD;
  tmSet.Hour = hh;
  tmSet.Minute = mm;
  tmSet.Second = ss;
  return makeTime(tmSet);
  

/*
    struct tm t;
    time_t t_of_day;

    t.tm_year = YYYY-1900;
    t.tm_mon = MM;           // Month, 0 - jan
    t.tm_mday = DD;          // Day of the month
    t.tm_hour = hh;
    t.tm_min = mm;
    t.tm_sec = ss;
    t.tm_isdst = 1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
    t_of_day = mktime(&t);

    //display.print("seconds since the Epoch:", (long) t_of_day)
    return t_of_day;
*/

}


