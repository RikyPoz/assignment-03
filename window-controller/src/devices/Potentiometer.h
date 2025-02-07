#ifndef __POTENTIOMETER__
#define __POTENTIOMETER__

class Potentiometer
{

public:
    Potentiometer(int pin);
    int detectValue();

private:
    int pin;
};

#endif