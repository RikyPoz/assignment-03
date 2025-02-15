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

void Display::defaultMessage()
{
    lcd->clear();
    lcd->setCursor(2, 0);
    lcd->print("Mode: ");
    lcd->setCursor(2, 1);
    lcd->print("Temp (C): ");
    lcd->print("0");
    lcd->setCursor(2, 2);
    lcd->print("Window (%): ");
    lcd->print("0");
}

void Display::updateMode(char* mode)
{
    lcd->setCursor(8,0);
    lcd->print("         ");
    lcd->setCursor(8,0);
    lcd->print(mode);
}

void Display::updateTemperature(float temp)
{
    char charTemp[5];
    lcd->setCursor(2, 1);
    lcd->print("Temp (C): ");
    lcd->print(dtostrf(temp, 3, 1, charTemp));
    lcd->print("  ");
}

void Display::updateLevel(int windowLevel)
{
    char windowBuffer[4];
    lcd->setCursor(2, 2);
    lcd->print("Window (%): ");
    lcd->print(itoa(windowLevel, windowBuffer, 10));
    lcd->print("  ");
}

void Display::noTemperature() {
    lcd->setCursor(2,1);
    lcd->print("               ");
}
