#include "Termo.h"
#include <Arduino.h>

Termo::Termo(int pin)
{
  this->pin = pin;
  pinMode(pin, INPUT);
}

int Termo::detectValue()
{

  float voltage = analogRead(pin) * 3.3 / 4095.0;
  float temperatureC = (voltage - 0.5) / 0.01;
  return temperatureC;
}