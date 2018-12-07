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

// mapping suggestion for AVR, UNO, NANO etc.
// BUSY -> 7, RST -> 9, DC -> 8, CS-> 10, CLK -> 13, DIN -> 11


#include <WiFi.h>
#include <PubSubClient.h>


// include library, include base class, make path known
#include <GxEPD.h>


#include <GxGDEW042Z15/GxGDEW042Z15.h>   // 4.2" b/w/r
//#include <GxGDEW042T2/GxGDEW042T2.h>      // 4.2" b/w

#include <HTU21D.h>

HTU21D sensor; // Instanz für HTU21D



// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>

//#include "BitmapGraphics.h"

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

GxIO_Class io(SPI, /*CS=5*/ SS, /*DC=*/ 32, /*RST=*/ 33); // arbitrary selection of 17, 16
GxEPD_Class display(io, /*RST=*/ 33, /*BUSY=*/ 25); // arbitrary selection of (16), 4


// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;

String MQTT_Message = "";

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "";
const char* password = "";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.1.102";

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
	delay(10);
	// We start by connecting to a WiFi network
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.print("WiFi connected - ESP IP address: ");
	Serial.println(WiFi.localIP());
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
	// Loop until we're reconnected
	while (!client.connected()) {
		Serial.print("Attempting MQTT connection...");
		// Attempt to connect
		/*
		YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
		To change the ESP device ID, you will have to give a new name to the ESP8266.
		Here's how it looks:
		if (client.connect("ESP8266Client")) {
		You can do it like this:
		if (client.connect("ESP1_Office")) {
		Then, for the other ESP:
		if (client.connect("ESP2_Garage")) {
		That should solve your MQTT multiple connections problem
		*/
		if (client.connect("ESP32Client")) {
			Serial.println("connected");
			// Subscribe or resubscribe to a topic
			// You can subscribe to more topics (to control more LEDs in this example)
			client.subscribe("test_channel");
		}
		else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}


// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
	Serial.print("Message arrived on topic: ");
	Serial.print(topic);
	Serial.print(". Message: ");
	String messageTemp;

	for (int i = 0; i < length; i++) {
		Serial.print((char)message[i]);
		messageTemp += (char)message[i];
	}
	MQTT_Message = messageTemp;
	Serial.println(messageTemp);

}


void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  sensor.begin();

  display.init(115200); // enable diagnostic output on Serial

  Serial.println("setup done");
}

void loop()
{
	if (!client.connected()) {
		reconnect();
	}
	if (!client.loop())
		client.connect("ESP32Client");

	now = millis();
	// Publishes new temperature and humidity every 30 seconds
	if (now - lastMeasure > 60000) {
		lastMeasure = now;
		const GFXfont* f = &FreeMonoBold24pt7b;
		display.fillScreen(GxEPD_WHITE);
		display.setTextColor(GxEPD_RED);
		display.setCursor(0, 30);
		display.setFont(f);
		display.println("MQTT");
		f = &FreeMonoBold12pt7b;
		display.setFont(f);
		display.setTextColor(GxEPD_BLACK);
		display.setCursor(0, 100);
		char temperature_[7];
		dtostrf(sensor.readTemperature(), 3, 1, temperature_);
		client.publish("test_channel", temperature_);
		display.print(temperature_);
		display.setCursor(0, 200);
		display.print(MQTT_Message);
		display.update();
		}
		
	
		
}

void showPartialUpdate(float temperature)
{
	String temperatureString;
	if (temperature>100)
	{
		temperatureString = String(temperature, 0);
		
	}
	else
	{
		temperatureString = String(temperature, 1);
	}
	temperatureString += " °C";
	const GFXfont* f = &FreeSansBold24pt7b;

	uint16_t box_x = 60;
	uint16_t box_y = 60;
	uint16_t box_w = 120;
	uint16_t box_h = 60;
	uint16_t cursor_y = box_y + 16;

	display.setRotation(0);
	display.setFont(f);
	display.setTextColor(GxEPD_BLACK);

	display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
	display.setCursor(box_x, cursor_y + 38);
	display.print(temperatureString);
	display.updateWindow(box_x, box_y, box_w, box_h);

	char temperature_[7];
	dtostrf(temperature, 3, 1, temperature_);


	client.publish("test_channel", temperature_);

}