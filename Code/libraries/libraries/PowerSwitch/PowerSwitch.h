#pragma once

#include <Actor.h>

/*
	PowerSwitch ist ein spezieller sehr einfacher Aktor als
	Ein/Ausschalter über einen digitalen Ausgang.
*/
class PowerSwitch : public Actor
{
 public:
	 PowerSwitch(const char* nodeName, const char* name, int pinNumber);
	 virtual void changeActorTo(float value);
private:
	int _pinNumber;
};

