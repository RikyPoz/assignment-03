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
    void updateMode(String mode);
    void updateLevel(String windowLevel);
    void updateTemperature(String temp);

private:
    LiquidCrystal_I2C *lcd;
};

#endif
