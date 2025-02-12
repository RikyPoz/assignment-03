#include "Potentiometer.h"
#include <Arduino.h>

Potentiometer::Potentiometer(int pin)
{
    this->pin = pin;
    pinMode(pin, INPUT);
}

int Potentiometer::detectValue()
{
    int val = analogRead(pin);
    return map(val, 0, 1023, 0, 100);
}