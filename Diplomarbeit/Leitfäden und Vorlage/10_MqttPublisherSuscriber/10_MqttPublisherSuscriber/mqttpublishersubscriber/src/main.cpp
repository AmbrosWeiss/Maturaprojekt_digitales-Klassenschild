#include <HttpServer.h>
#include <PubSubClient.h>
#include <NodeTime.h>
#include <NodeConfig.h>
#include <MqttClient.h>

static void mqttSubscriberCallback(const char *topic, const char *payload)
{
	Serial.print(F("*MT: Received, Topic: "));
	Serial.print(topic);
	Serial.print(F(", Payload: "));
	Serial.println(payload);
}

MqttSubscription timeSubscription;
char timeTopic[] = "time";
MqttSubscription nodeSubscription;
char nodeTopic[50];

/********************************** Mqtt - Config ***************************/
void handleRoot(){
	//Macht eine kleine Form zum visuellen Editieren der configs. die /setconfig-Route ist bereits im
	//HttpServer definiert.
	char response[] = "<form action=/setconfig>Server<br><input placeholder='zB 192.168.1.xx' name=server><br>Port<br> "
	"<input value=1883 name=port><br>Password<br><input placeholder='MQTT username' name=password><br>Nodename<br> "
	"<input placeholder='MQTT password' name=nodename><br><input value=Submit type=submit></form>";
	HttpServer.send(200, "text/html", response);
}

/*************************************** Setup ******************************/
void setup()
{
	Serial.begin(115200); //Initialisierung der seriellen Schnittstelle
	Serial.println();
	Serial.println();
	Serial.println(F("NodeTest fuer MQTT"));
	Serial.println(F("=================="));
	HttpServer.init();							   // HttpServer initialisieren
	HttpServer.on("/", handleRoot);
	NodeTime.setNtpTimeSubscriber();			   // Zeit über Internet synchronisieren
	MqttClient.init(NodeConfig.getMqttNodeName()); // MqttClient initialisieren
	timeSubscription.topic = timeTopic;			   // Subscription auf 'time' für Zeitsynchronisierung
	timeSubscription.subscriberCallback = mqttSubscriberCallback;
	MqttClient.addSubscription(&timeSubscription);
	strcpy(nodeTopic, NodeConfig.getMqttNodeName());
	nodeSubscription.topic = nodeTopic;
	nodeSubscription.subscriberCallback = mqttSubscriberCallback; // subscription auf eigenen NodeName
	MqttClient.addSubscription(&nodeSubscription);
	MqttClient.subscribeToBroker();
}

/*************************************** Loop ******************************/
char topic[50];
long lastPublishTime = 0;
char longTimeText[15];

void loop()
{
	HttpServer.handleClient();
	MqttClient.doLoop(); // Mqtt-Schnittstelle bedienen
	if (millis() - lastPublishTime > 5000)
	{
		sprintf(longTimeText, "%d", NodeTime.getDateTime());
		// sprintf(topic, "%s/time", NodeConfig.getMqttNodeName());
		MqttClient.publish(NodeConfig.getMqttNodeName(), longTimeText);
		Serial.print("*MT: publish to ");
		Serial.print(NodeConfig.getMqttNodeName());
		Serial.print(": ");
		Serial.println(longTimeText);
		lastPublishTime = millis();
	}
	delay(1);
}
