// WifiHttpServer.h
#pragma once

#include <arduino.h>
#include <ESP8266WebServer.h>

static char PHSEM_AP_NAME[] = "HIER MEINEN NAMEN EINTRAGEN";

class HttpServerClass : public ESP8266WebServer
{
 public:
	HttpServerClass(int port = 80);
	void init();
 private:
};

extern HttpServerClass HttpServer;

