
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
//#include <GxGDEW075T8/GxGDEW075T8.h>      // 7.5" b/w



#include <GxGDEW075Z09/GxGDEW075Z09.h>    // 7.5" b/w/r
#include "img/Rahmen13.h"
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

 char* id="40";                     //id der Raumnummer
 char* ssid ="AndroidAP07A0";
 char* password ="Philipp99";
 char* mqttServer ="192.168.43.248";
 const int mqttPort = 1883;
 String Nachricht[100];             // Array für Nachricht 
 int a = 0;                       // Um in die If-Bedingung im Main zu springen
 char string[300];
 int x;

  WiFiClient espClient;
  PubSubClient client(espClient);

GxIO_Class io(SPI, /*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16); // arbitrary selection of 17, 16
GxEPD_Class display(io, /*RST=*/ 16, /*BUSY=*/ 4); // arbitrary selection of (16), 4

String split(String s, char parser, int index);

void setup() {
  
  Serial.begin(115200); 
  display.init(115200); // enable diagnostic output on Serial
  WiFi.begin(ssid, password);
  
  display.drawBitmap(Rahmen13, sizeof(Rahmen13));
  
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  
    Serial.println("Connected to the WiFi network");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

 
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client")) {
 
      Serial.println("connected");  
 
    } 
    else {
 
      Serial.print("failed with state ");
      Serial.println(client.state());
      delay(2000);
 
    }
  }

  client.publish("HTL/Klasse/id", (const char*)id);
  client.subscribe("HTL/Klasse");
  
 
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
    

    
   // a= 1;
  
}

void loop() {
  
  delay(1);
  
  if (!client.connected()) {
    reconnect();
  }

 
  
  if(a==1){
    display.BitmapToBuffer(Rahmen13, sizeof(Rahmen13));
    display.setTextSize(4);
    display.setTextColor(GxEPD_BLACK, GxEPD_WHITE);
    display.setCursor(220,110);
    //display.print(Nachricht[1]);
    display.update();
    a = 0;
    Serial.println("-----------------------");
    Serial.println("Fertig!");
    Serial.println("-----------------------");
  }
  
  client.loop();



}

