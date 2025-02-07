#include <Arduino.h>
#include "Display.h"

Display::Display(LiquidCrystal_I2C *lcd)
{
    this->lcd = lcd;
}

void Display::init()
{
    lcd->init();
}

void Display::turnOnDisplay()
{
    lcd->backlight();
    lcd->clear();
}

void Display::turnOffDisplay()
{
    lcd->noBacklight();
}

void Display::automaticMessage()
{
    lcd->clear();
    lcd->setCursor(4, 1);
    lcd->print("AUTOMATIC MODE");
}

void Display::manualMessage(String temp)
{
    lcd->clear();
    lcd->setCursor(4, 1);
    lcd->print("MANUAL MODE");
    lcd->setCursor(4, 2);
    lcd->print(temp);
}
