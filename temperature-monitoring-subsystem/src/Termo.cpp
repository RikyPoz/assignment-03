#include "Termo.h"
#include <Arduino.h>

Termo::Termo(int pin)
{
  this->pin = pin;
  pinMode(pin, INPUT);
}

int Termo::detectValue()
{
  return (analogRead(pin) * 0.488) - 50;
}