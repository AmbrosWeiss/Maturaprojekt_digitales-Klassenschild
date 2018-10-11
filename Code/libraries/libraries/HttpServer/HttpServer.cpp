#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include "HttpServer.h"
#include <InternLed.h>
#include <WiFiManager.h>
#include <NodeConfig.h>
//#include <ArduinoOTA.h>


HttpServerClass::HttpServerClass(int port) : ESP8266WebServer(port) {
}

/*************************************** Callback-Funktionen für den Webserver ******************************/

/**
 * Verarbeitung der Übermittlung der MQTT-Zugangsdaten
 * Format: http://192.168.0.12/setconfig?server=192.168.0.11&port=1833&user=XXXX&password=YYYY&nodename=aaaa
 */
void handleSetMqttConfigRequest() { 
	Serial.println(F("*HS: handleSetMqttConfigRequest()"));
	String server = HttpServer.arg("server");
	String port = HttpServer.arg("port");
	String user = HttpServer.arg("user");
	String password = HttpServer.arg("password");
	String nodeName = HttpServer.arg("nodename");
	char serverChars[MQTT_SERVERADDRESS_LENGTH];
	char portChars[MQTT_PORT_LENGTH];
	char userChars[MQTT_USERNAME_LENGTH];
	char passwordChars[MQTT_PASSWORD_LENGTH];
	char nodeNameChars[MQTT_NODE_NAME_LENGTH];

	int changes = 0;
	if (!server.equals("")){     //Parameter found
		server.toCharArray(serverChars, MQTT_SERVERADDRESS_LENGTH);
		NodeConfig.setMqttServerAddress(serverChars);
		changes++;
	}
	if (!port.equals("")){
		port.toCharArray(portChars, MQTT_PORT_LENGTH);
		NodeConfig.setMqttPort(portChars);
		changes++;
	}
	if (!user.equals("")){
		user.toCharArray(userChars, MQTT_USERNAME_LENGTH);
		NodeConfig.setMqttUserName(userChars);
		changes++;
	}
	if (!password.equals("")){
		password.toCharArray(passwordChars, MQTT_PASSWORD_LENGTH);
		NodeConfig.setMqttPassword(passwordChars);
		changes++;
	}
	if (!nodeName.equals("")){
		nodeName.toCharArray(nodeNameChars, MQTT_NODE_NAME_LENGTH);
		NodeConfig.setMqttNodeName(nodeNameChars);
		changes++;
	}
	if(changes > 0){
		NodeConfig.saveConfig();
	}
	char response[50];
	sprintf(response, "Changed MQTT-Config-Items: %d",changes);
	Serial.print("*HS: ");
	Serial.println(response);
	HttpServer.send( 200, "text/html", response);          //Returns the HTTP response
}

/*
	Umwandlung einer Hexziffer (0-15) in das Hexzeichen (0-F)
*/
char getHexChar(int digit) {
	if (digit < 10) {
		return '0' + digit;
	}
	return 'A' + digit - 10;
}

/*
  MAC-Adresse in entsprechenden String umwandeln
  und in übergebenes char[] speichern
*/
void getMacAddress(char hexChars[]) {
	uint8_t *bssid = WiFi.BSSID();
	for (int i = 0; i < 6; i++) {
		hexChars[i * 3] = getHexChar(bssid[i] / 16);
		hexChars[i * 3 + 1] = getHexChar(bssid[i] % 16);
		hexChars[i * 3 + 2] = ':';
	}
	hexChars[17] = 0;
}

/*
	ESP meldet Statusinformationen, wie Mac-Adresse, SSID, WLAN-Funk-Feldstärke,
	freien Heap-Speicher, den eigenen NodeName, MQTT-Username, MQTT-Broker-IP,
	MQTT-Broker-Port
		http://192.168.0.100/state
*/
void handleStateRequest() {
	char hexChars[18];
	char response[200];
	getMacAddress(hexChars);

	sprintf(response, "espMacAddress: %s<br/>wifiName: %s<br/>wifiStrength: %d<br/>freeHeap: %d<br/>Node-Name: %s<br/>MQTT-User: %s<br/>MQTT-Broker: %s:%s",
				hexChars, WiFi.SSID().c_str(), WiFi.RSSI(), ESP.getFreeHeap()
				, NodeConfig.getMqttNodeName()
				, NodeConfig.getMqttUserName()
				, NodeConfig.getMqttServerAddress()
				, NodeConfig.getMqttPort());

    Serial.printf("*HS response-length: %d",strlen(response));
	Serial.println("");
	Serial.print(F("*HS: State: "));
	Serial.println(response);
	//Returns the HTTP response
	HttpServer.send( 200, "text/html", response);          
}

/**
 * Get the current configuration
 * */
void handleGetConfigRequest() {
	char response[100];
	NodeConfig.getConfigText(response);
	Serial.print(F("*HS: State: "));
	Serial.print(response);
	//Returns the HTTP response
	HttpServer.send( 200, "text/html", response);          
}

/**
 * Die interne Led kann per Http gesteuert werden
 * Mit dem Parameter fast=1 blinkt die LED schnell,
 * mit slow=1 blinkt sie langsam und mit off=1 wird
 * sie abgeschaltet
 *		http://192.168.0.100/led/fast
 *		http://192.168.0.100/led/slow
 *		http://192.168.0.100/led/off
*/
void handleInternLedFastRequest() {
	InternLed.blinkFast();
	Serial.println(F("LED blinks fast"));
	HttpServer.send( 200, "text/html", "LED blinks fast"); 
}

void handleInternLedSlowRequest() {
	InternLed.blinkSlow();
	Serial.println(F("LED blinks slow"));
	HttpServer.send( 200, "text/html", "LED blinks slow"); 
}

void handleInternLedOffRequest() {
	InternLed.blinkOff();
	Serial.println(F("LED off"));
	HttpServer.send( 200, "text/html", "LED off"); 
}

// void handleInternLedRequest() {
// 	String response = "";
// 	if(HttpServer.args() > 0){
// 		String ledParName = HttpServer.argName(0);
// 		Serial.println("ledParName: "+ledParName);
// 		if(ledParName.equals("fast")) {
// 			InternLed.blinkFast();
// 			response="Led blink fast";
// 		} else if (ledParName.equals("slow")) {
// 			InternLed.blinkSlow();
// 			response="Led blink slow";
// 		} else if (ledParName.equals("off")) {
// 			InternLed.blinkOff();
// 			response="Led blink off";
// 		} else {
// 			response="Illegal led-command "+ledParName;
// 		}
// 	}
// 	else{
// 		response = "led-command no parameter found";
// 	}
// 	Serial.println("Led-Command, "+response);
// 	HttpServer.send( 200, "text/html", response);          //Returns the HTTP response
// }

/*
	Credentials für das WLAN werden zurückgesetzt
	SPIFFS wird formatiert
*/
void handleResetEspRequest() { 
	InternLed.blinkSlow();
	const char* response = "Formating the FlashFileSystem<br>Wifi-Connection reset settings, reboot ESP";
	HttpServer.send( 200, "text/html", response);          //Returns the HTTP response
	delay(1000);
	Serial.println(F("*HS Format flash memory!"));
    SPIFFS.format();
    Serial.println(F("*HS Done!"));
    WiFi.disconnect(true);     
	WiFiManager wifiManager;
	Serial.println(F("*HS WifiManager resetSettings()"));
	wifiManager.resetSettings();
    Serial.println(F("*HS Done!"));
	InternLed.blinkOff();
}

/*************************************** Allgemeine HttpServer-Methoden ******************************/

//flag for saving data
bool saveConfig = false;

//callback notifying us of the need to save config
void saveConfigToFileCallback () {
  Serial.println("*HS Should save config");
  saveConfig = true;
}


/*
	Accesspoint definieren und öffnen. Webserver starten und auf Get-Request warten
*/
void HttpServerClass::init()
{
	// Konfiguration aus Flashspeicher einlesen
	NodeConfig.readConfig();
	wifi_station_set_hostname(NodeConfig.getMqttNodeName());  // damit er per Fing auch gefunden wird
	//ArduinoOTA.setHostname(NodeConfig.getMqttNodeName());

	// WiFiManger vorbereiten für Konfiguration per Webseite
	WiFiManagerParameter custom_mqtt_server("server", "mqtt server", NodeConfig.getMqttServerAddress(), MQTT_SERVERADDRESS_LENGTH);
	WiFiManagerParameter custom_mqtt_port("port", "mqtt port", NodeConfig.getMqttPort(), MQTT_PORT_LENGTH);
	WiFiManagerParameter custom_mqtt_user("user", "mqtt user", NodeConfig.getMqttUserName(), MQTT_USERNAME_LENGTH);
	WiFiManagerParameter custom_mqtt_pass("pass", "mqtt pass", NodeConfig.getMqttPassword(), MQTT_PASSWORD_LENGTH);
	WiFiManagerParameter custom_mqtt_node_name("nodeName", "mqtt node name", NodeConfig.getMqttNodeName(), MQTT_NODE_NAME_LENGTH);
	WiFiManager wifiManager;				// WiFiManager anlegen
	//wifiManager.autoConnect(PHSEM_AP_NAME);

	wifiManager.setSaveConfigCallback(saveConfigToFileCallback);

	wifiManager.addParameter(&custom_mqtt_server);
	wifiManager.addParameter(&custom_mqtt_port);
	wifiManager.addParameter(&custom_mqtt_user);
	wifiManager.addParameter(&custom_mqtt_pass);
	wifiManager.addParameter(&custom_mqtt_node_name);
	// Namen des AP mit Suffix MAC-Adresse erzeugen
	char hexChars[18];						// Macadresse ermitteln
	getMacAddress(hexChars);
	char apName[25];
	sprintf(apName, "ESP_%s", hexChars);   // SSID zusammenstoppeln
	InternLed.blinkFast();				   // Signal dafür, dass ESP mit eigenem AP auf 192.168.4.1 wartet	

	bool isConnected = true;
    wifiManager.setConfigPortalTimeout(240);
//	if (!wifiManager.autoConnect(apName))  // gecachte Credentials werden verwendet
	if (!wifiManager.autoConnect(PHSEM_AP_NAME))  // gecachte Credentials werden verwendet
	{
		// Serial.println(F("Failed to connect. Reset and try again..."));
		isConnected=false;
		// delay(3000);
		// ESP.reset();						//reset and try again
		// delay(5000);
	}
	delay(100);
	if (saveConfig) {
		InternLed.blinkSlow();
		// read updated parameters
		// save to configfile
		Serial.println(F("*HS: Save config-values to file"));
		NodeConfig.setMqttServerAddress(custom_mqtt_server.getValue());
		NodeConfig.setMqttPort(custom_mqtt_port.getValue());
		NodeConfig.setMqttUserName(custom_mqtt_user.getValue());
		NodeConfig.setMqttPassword(custom_mqtt_pass.getValue());
		NodeConfig.setMqttNodeName(custom_mqtt_node_name.getValue());
		NodeConfig.saveConfig();
		delay(2000);
		InternLed.blinkOff();
	}
	if(isConnected){
		Serial.println(F("*HS: Connected to WiFi!"));
		Serial.print(F("*HS: Hostname: "));
		Serial.println(wifi_station_get_hostname());
		InternLed.blinkOff();
		//InternLed.blinkSlow();
		on("/setconfig", handleSetMqttConfigRequest);	// MQTT-Zugangsdaten per Web setzen
		on("/reset", handleResetEspRequest); 		// ESP resetten
		on("/state", handleStateRequest); 		// Status (Heap, Funkfeldstärke, ...) ausgeben
		on("/led/fast", handleInternLedFastRequest);
		on("/led/slow", handleInternLedSlowRequest);
		on("/led/off", handleInternLedOffRequest);
		on("/getconfig", handleGetConfigRequest);
		begin();
		// Serial.println(F("*HS: Webserver started"));
		delay(3000);
		Serial.println(F("*HS: Webserver started"));
	} else{
		Serial.println(F("!HS: not connected to WiFi!"));
		delay(3000);
		ESP.reset();						//reset and try again
	}

}

// Quasi Singletonimplementierung
HttpServerClass HttpServer;
