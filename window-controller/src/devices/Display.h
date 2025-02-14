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
    void defaultMessage();
    void updateMode(char mode[10]);
    void updateLevel(char windowLevel[4]);
    void updateTemperature(char temp[5]);

private:
    LiquidCrystal_I2C *lcd;
};

#endif
