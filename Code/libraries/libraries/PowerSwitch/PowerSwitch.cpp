#include <Logger.h>
#include <PowerSwitch.h>
#include <PowerSwitchSensor.h>
#include <Node.h>

/*
	Der Schalter hat keine Einheit des Messwertes EIN/AUS und auch eine fixe Schwelle von 0.001,
	da sowieso nur die Werte 0 und 1 verwendet werden.
*/
PowerSwitch::PowerSwitch(const char *nodeName, const char *name, int pinNumber) : Actor(nodeName, name)
{
	_sensorPtr = new PowerSwitchSensor(nodeName, name, "", 0.001, pinNumber);
	_pinNumber = pinNumber;
	_settedValue = 0;
	pinMode(_pinNumber, OUTPUT);
	digitalWrite(_pinNumber, 1); // inverse Logik
}

void PowerSwitch::changeActorTo(float value)
{
	if (value > 0.1)
	{
		Serial.println(F("*PS: set pin low!"));
		digitalWrite(_pinNumber, 0); // inverse Logik
	}
	else
	{
		Serial.println(F("*PS: set pin high!"));
		digitalWrite(_pinNumber, 1);
	}
}
