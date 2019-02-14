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
//eigentlich das Richtige, aber da ich eine eigene Funktion geschrieben habe, benutze ich diese lokal
//#include <GxGDEW075Z09/GxGDEW075Z09.h>    // 7.5" b/w/r

#include <GxGDEW075Z09/GxGDEW075Z09.h>    // 7.5" b/w/r

//Einfügen der Bitmap
#include "img/Rahmen13.h"

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>



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




//__________________MQTT__________

#include <PubSubClient.h>
  WiFiClient espClient;
  PubSubClient client(espClient);
  char* mqttServer ="192.168.43.248";
  const int mqttPort = 1883;
  // Array für Nachricht 
  String Nachricht[100];             
  // Um in die If-Bedingung im Main zu springen
  int a = 0;                       
  char string[300];
  int x;
  //id der Raumnummer
  char* id="40";                     
//_______________MQTT_ENDE




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




//_______________________MQTT_____________________________
Serial.println(WiFi.localIP()); 

    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);


  while (!client.connected()) 
  {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client")) 
    { 
      Serial.println("connected");  
    } 
    else 
    {
      Serial.print("failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }

   // Anfrage für die Klasse mit der Id
   client.publish("HTL/Klasse/id", (const char*)id); 
   // Angefordere Daten werden Auf HTL/Klasse gepublished
   client.subscribe("HTL/Klasse");                   
//____________________MQTT_ENDE___________________________________

}






long int getSleepingTime()
{
  //ruft die aktuelle Stunde auf
  int aktuelleStunde = timeClient.getHours();
  //ruft die aktuelle Minute auf
  int aktuelleMinute = timeClient.getMinutes();
  //ruft den aktuellen Wochentag (Montag, ...)auf
  int aktuellerWochentag = timeClient.getDay();

  //aktuelle Uhrzeit und Datum in Epoch-Format
  unsigned long sec = timeClient.getEpochTime();



  //Wenn es Mo-Fr ist --> aktualiesierungszyklen nach dem Array wakeuptimeWeekdays
  if(aktuellerWochentag > 0 && aktuellerWochentag <=5)
  {
      //wenn die vorgegebene Aufwachzeit in der aktuellen Stunde (z.B. Stunde 10) im Array ander Stelle (9+1) größer ist,
      //als die aktuelle Zeit, so wird die vorgegebene Zeit minus der aktuellen Zeit gerechnet 
      //Beispiel: vorg. 10:46 / akt. 10:30 --> 10:46 > 10:30 --> 46-30 = 16 --> 16 * 60 --> 960 Sekunden schlafen legen
      if(wakeuptimeWeekdays[aktuelleStunde + 1] >= aktuelleMinute)
      {
        long int schlafdauer = wakeuptimeWeekdays[aktuelleStunde+1] - aktuelleMinute;
        long int schlafdauerSek = schlafdauer * 60;
        return schlafdauerSek;
      }

      
      //vorgegebene Zeit kleiner als akt. Zeit, Stunde aber eine anderere (Bsp: vorg. 10:46 / akt. = 10:50) --> stunde wird erhöht, weil er somit erst in der nächsten Stunde
      //in diesem Beispiel um 11:41 aufwachen muss --> die Werte werden in echo-Code umgewandelt und wieder die vorgegebene Zeit minus der akutellen Zeit gerechent, so erhält man auch wieder die Zeit, wie lange der ESP schlafen muss
      if(wakeuptimeWeekdays[aktuelleStunde+1] <= aktuelleMinute)
      {
        //Uhrzeit und Datum in echo-Code
        /*time_t*/ long int aktuelleUhrzeit = tmConvert_t(timeClient.getYear(sec), timeClient.getMonth(sec), timeClient.getDayNumber(sec), timeClient.getHours(),timeClient.getMinutes(), timeClient.getSeconds());
        int aktStd = timeClient.getHours();

        //gibt an, wann er wieder aufwachen muss (gilt nur für unter der Woche)          
        //"aktStd+1", weil erst in der nächsten Stunde wieder aufwachen (oben angegeben)
        //"wakeuptimeWeekdays[aktuelleStunde+1]", so erhält man die Minute, wann er in der nächsten Stunde aufwachen muss
        /*time_t*/ long int aufwachzeit = tmConvert_t(timeClient.getYear(sec), timeClient.getMonth(sec), timeClient.getDayNumber(sec), aktStd+1, wakeuptimeWeekdays[aktuelleStunde+1], timeClient.getSeconds());

        //wie lange er schlafen darf
        //Wert sollte schon in Sekunden sein --> weil echo sekunden zurück gibt
        long int schlafdauer = aufwachzeit - aktuelleUhrzeit;
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




  //die Bitmap, also der Rahmen wird in den Black-Buffer gespeichert, in welchem in weiterer Folge auch der Text gespeichert wird
  //der Black-Buffer wird dann mit "display.update" ausgegeben
  display.BitmapToBuffer(Rahmen13, sizeof(Rahmen13));
  display.setTextSize(4);
  display.setTextColor(GxEPD_BLACK, GxEPD_WHITE);

  //ist der Client nicht verbunden, wird die Verbindung wieder gestartet
   if (!client.connected()) {
    reconnect();
  }




  /*__________NACHRICHTEN_ARRAY________________________________
  So ist das Nachrichten-Array aufgebaut
  ES MUSS IMMER +1 GERECHNET WERDEN
  
  Nachricht[0] Startzeit 
  Nachricht[1] Endzeit
  Nachricht[2] Klasse
  Nachricht[3] Lehrer
  Nachricht[4] Fach
  Nachricht[5] Raum 
  Nachricht[6] Stammklasse
  Nachricht[7] Prüfung  JA NEIN
  Nachricht[8] Suplierung
  Nachricht[9] nKlasse
  Nachricht[10] nLehrer
  Nachricht[11]nFach
  ______________________________________________________________
*/



if(a==1){

  unsigned long sec = timeClient.getEpochTime();

  //aktuelle Klasse
  display.setCursor(220,108);
  display.print(Nachricht[3]);

  
  //Raumnummer
  display.setTextSize(4);
  display.setCursor(220,295);
  display.print(Nachricht[6]);
  
  //Stammklasse
  display.setTextSize(2);
  display.setCursor(220,350);
  display.print(Nachricht[7]);

  //Datum
  //hier wird das (durch eigen geschriebene Funktion in der library) ausgeben
  display.setTextSize(1);
  //x,y
  display.setCursor(540,22);
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
  display.print(Nachricht[4]);
  
  //Gegenstand
  display.setTextSize(2);
  display.setCursor(570,187);
  display.print(Nachricht[5]);
  
  //nächster Lehrer
  display.setTextSize(2);
  display.setCursor(500,285);
  display.print(Nachricht[11]);
  
  //nächste Klasse
  display.setTextSize(2);
  display.setCursor(500,305);
  display.print(Nachricht[10]);
  
  //nächstes Fach
  display.setTextSize(2);
  display.setCursor(500,327);
  display.print(Nachricht[12]);

  display.update();
  delay(200000);

  a=0;

}
  
  

  client.loop();
  /*
  //Deep Sleep - WICHTIG - für Informationsnachmittag auskommentiert
  long int schlafdauer = getSleepingTime();
  schlafdauer=schlafdauer * 1000000;
  Serial.print("Schlafdauer: ");
  Serial.print(schlafdauer);
  esp_sleep_enable_timer_wakeup(schlafdauer);
  esp_deep_sleep_start();
  */
}




void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("HTL/Klasse/id", (const char*)id);
      // ... and resubscribe
      client.subscribe("HTL/Klasse");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("-----------------------");
  Serial.println("Message arrived in topic: ");
  Serial.println(topic);
 
    Serial.print("Message:");
  for (int i = 0; i <= length; i++) {
    Serial.print((char)payload[i]);
   string[i]= ((char)payload[i]);  // Die Empfangene Nachricht in ein Array speicher um sie ausgeben zu können
  }
  

char delimiter[] = {',',';','"','[',']'};
char *ptr;


// initialisieren und ersten Abschnitt erstellen
ptr = strtok(string, delimiter);
Nachricht[x];
x++;


while(ptr != NULL) {
  printf("Abschnitt gefunden: %s\n", ptr);
  Nachricht[x]= ptr;
  x++;
  // naechsten Abschnitt erstellen
  ptr = strtok(NULL, delimiter);
}
    Serial.println();
    Serial.println("-----------------------");
     
  a=1;
}
