#ifndef __TERMO__
#define __TERMO__

#include <Arduino.h>

class Termo
{

public:
  Termo(int pin);
  int detectValue();

private:
  int pin;
};

#endif