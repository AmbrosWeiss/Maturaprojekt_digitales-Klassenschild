//RTC-Versuch-ohne MQTT - nur unter Schultag
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
//eigentlich das Richtige, aber da ich eine eigene Funktion geschrieben habe, benutze ich diese lokal
//#include <GxGDEW075Z09/GxGDEW075Z09.h>    // 7.5" b/w/r

#include <GxGDEW075Z09/GxGDEW075Z09.h>    // 7.5" b/w/r

//Einfügen der Bitmap
#include "img/Rahmen13.h"

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>


//___________________DEEP_SLEEP____________
#define uS_TO_S_FACTOR 1000000
RTC_DATA_ATTR int bootCount = 0;
//___________________DEEP_SLEEP_ENDE______________



//____________________RTC__________________
#include <WiFi.h>
#include <NTPClientSeparated.h>

//damit Uhrzeit/Datum in epoch code umwandeln
#include "Time.h"

//für Wochentag
char* daysOfTheWeek[7] = {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};

//Netzwerk (WLAN) Daten
const char* ssid = "AndroidAP07A0";
const char* password = "Philipp99";

//NTP-Client definieren um Zeit zu bekommen
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

//Variablem um Uhrzeit und Datum zu speichern
String formattedDate;
String dayStamp;
String timeStamp;
//___________________RTC_ENDE______________




//___________________AUFWACHZEITEN_UNTER_DER_WOCHE___________________________________________________________
const char wakeuptimeWeekdays[24] = {0, 0, 0, 0, 0, 0, 55, 51, 46, 41, 46, 41, 36, 26, 21, 16, 06, 0,  0, 0, 0, 0, 0, 0};
//           Uhrzeit in Stunden:     0  1  2  3  4  5   6   7   8   9  10  11  12  13  14  15  16  17 18 19 20 21 22 23
//___________________________________________________________________________________________________________

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
    Serial.print("WiFi gestartet");
  }
  
  //start web server
  WiFi.localIP();

  //initialisiere NTPClient um Zeit zu bekommen
  timeClient.begin();

  //Offsett Zeit in Sekunden für die Richtige Zeitzone einistellen
  //GMT +1 = 3600
  timeClient.setTimeOffset(3600);
//_______________________RTC_ENDE_________________________






//_______________________DEEP_SLEEP_______________________
  
  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  Ausgabe();

  //gibt den Aufwachgrund für den ESP32 aus
  print_wakeup_reason();

  //Wandelt die angegebene Zeit in µs um
  long int schlafdauer = getSleepingTime();
  unsigned long int aufwachzeit = schlafdauer * uS_TO_S_FACTOR;
  Serial.print("\nAufwachen nach:");
  Serial.print(aufwachzeit);
  esp_sleep_enable_timer_wakeup(aufwachzeit);
  Serial.println("\nSetup ESP32 to sleep for every " + String(schlafdauer) +  " Seconds");
  //startet den DeepSleepMode
  esp_deep_sleep_start();
//_____________________DEEP_SLEEP_ENDE____________________
}



void loop() 
{
}

void Ausgabe()
{
  

//_____________________RTC________________________________
//manchmal ruft der NTPClient 1970 auf, um sicherzustellen --> nicht passiert --> Update erzwingen
  while(!timeClient.update()){
    timeClient.forceUpdate();
  }
//_______________________RTC_ENDE_________________________________



  //die Bitmap, also der Rahmen wird in den Black-Buffer gespeichert, in welchem in weiterer Folge auch der Text gespeichert wird
  //der Black-Buffer wird dann mit "display.update" ausgegeben
  //display.drawBitmap(Rahmen13, sizeof(Rahmen13));
  display.BitmapToBuffer(Rahmen13, sizeof(Rahmen13));
  display.setTextSize(4);
  display.setTextColor(GxEPD_BLACK, GxEPD_WHITE);



  unsigned long sec = timeClient.getEpochTime();

  //aktuelle Klasse
  display.setCursor(220,108);
  display.print("5B");

  
  //Raumnummer
  display.setTextSize(4);
  display.setCursor(220,295);
  display.print("318");
  
  //Stammklasse
  display.setTextSize(2);
  display.setCursor(220,350);
  display.print("5B");

  //Datum
  display.setTextSize(1);
  //x,y
  display.setCursor(540,22);
  /*display.print(dayStamp);*/
    //aktueller Wochentag
  display.print(daysOfTheWeek[timeClient.getDay()]);

  display.print(" ");
  
  display.print(timeClient.getDayNumber(sec));
  display.print(".");
  display.print(timeClient.getMonth(sec));
  display.print(".");
  display.print(timeClient.getYear(sec));
  

  
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
  display.print("Wolf");
  
  //nächste Klasse
  display.setTextSize(2);
  display.setCursor(500,305);
  display.print("3C");
  
  //nächstes Fach
  display.setTextSize(2);
  display.setCursor(500,327);
  display.print("HWE");

  display.update();
  delay(200);
}





long int getSleepingTime()
{

  long int schlafdauer;
  //ruft die aktuelle Stunde auf
  int aktuelleStunde = timeClient.getHours();
  int aktStd = timeClient.getHours();
  Serial.print("\n\n");
  Serial.print(aktuelleStunde);
  //ruft die aktuelle Minute auf
  int aktuelleMinute = timeClient.getMinutes();
  Serial.print(aktuelleMinute);
  Serial.print(timeClient.getSeconds());
  Serial.print("\n\n");
  //ruft den aktuellen Wochentag (Montag, ...)auf
  int aktuellerWochentag = timeClient.getDay();



  //aktuelle Uhrzeit und Datum in Epoch-Format
  unsigned long sec = timeClient.getEpochTime();

  long int aktuelleUhrzeit = timeClient.getEpochTime();




  //Wenn es Mo-Fr ist --> Aktualiesierungszyklen nach dem Array wakeuptimeWeekdays

//___________________AUFWACHZEITEN_UNTER_DER_WOCHE___________________________________________________________
//const char wakeuptimeWeekdays[24] = {0, 0, 0, 0, 0, 0, 55, 51, 46, 41, 46, 41, 36, 26, 21, 16, 06, 0,  0, 0, 0, 0, 0, 0};
//             Uhrzeit in Stunden:     0  1  2  3  4  5   6   7   8   9  10  11  12  13  14  15  16  17 18 19 20 21 22 23
//___________________________________________________________________________________________________________


    
      //wenn die vorgegebene Aufwachzeit in der aktuellen Stunde (z.B. Stunde 10) im Array ander Stelle (9+1) größer ist,
      //als die aktuelle Zeit so wird die Vorgegebene minus der aktuellen Zeit gerechnet 
      //Beispiel: vorg. 10:46 / akt. 10:30 --> 10:46 > 10:30 --> 46-30 = 16 --> 16 * 60 --> 960 Sekunden schlafen legen
      if(wakeuptimeWeekdays[aktuelleStunde /*+ 1*/] >= aktuelleMinute)
      {
        int schlafdauerMin = wakeuptimeWeekdays[aktuelleStunde /*+1*/] - aktuelleMinute;
        schlafdauer = schlafdauerMin * 60;
        return schlafdauer;
      }


      
      //vorgegebene Zeit kleiner als akt. Zeit, Stunde aber eine anderere (Bsp: vorg. 10:46 / akt. = 10:50) --> stunde wird erhöht, weil er somit erst in der nächsten Stunde
      //in diesem Beispiel um 11:41 aufwachen muss --> die Werte werden in echo-Code umgewandelt und wieder die vorgegebene - aktuelle
      if(wakeuptimeWeekdays[aktuelleStunde/*+1*/] <= aktuelleMinute)
      {
        //Uhrzeit und Datum in echo-Code
        ///*time_t*/ long int aktuelleUhrzeit = tmConvert_t(timeClient.getYear(sec), timeClient.getMonth(sec), timeClient.getDayNumber(sec), timeClient.getHours(),timeClient.getMinutes(), timeClient.getSeconds());

        //gibt an, wann er wieder aufwachen muss (gilt nur für unter der Woche)          
        //"aktStd+1", weil erst in der nächsten Stunde wieder aufwachen (oben angegeben)
        //"wakeuptimeWeekdays[aktuelleStunde+1]", so erhält man die Minute, wann er in der nächsten Stunde aufwachen muss
        /*time_t*/ long int aufwachzeit = tmConvert_t(timeClient.getYear(sec), timeClient.getMonth(sec), timeClient.getDayNumber(sec), aktStd+1, wakeuptimeWeekdays[aktStd+1], timeClient.getSeconds());


        //wie lange er schlafen darf
        //Wert sollte schon in Sekunden sein --> weil echo sekunden zurück gibt
        schlafdauer = aufwachzeit - aktuelleUhrzeit;
        return schlafdauer;
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
}



//https://lastminuteengineers.com/esp32-deep-sleep-wakeup-sources/
//diese Funktion gibt den Grund an, warum der ESP aufgewacht ist
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
    case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 2  : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  : Serial.println("Wakeup caused by timer"); break;
    case 4  : Serial.println("Wakeup caused by touchpad"); break;
    case 5  : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}






