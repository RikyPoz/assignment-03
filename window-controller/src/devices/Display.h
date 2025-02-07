#ifndef __DISPLAY__
#define __DISPLAY__

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class Display
{

public:
    Display(LiquidCrystal_I2C *lcd);
    void init();
    void turnOffDisplay();
    void turnOnDisplay();

    // Displayable messages
    void automaticMessage();
    void manualMessage(String temp);

private:
    LiquidCrystal_I2C *lcd;
};

#endif
