#include "Window.h"
#include "const.h"
#include <Arduino.h>
#include <avr/sleep.h>

Window::Window(ButtonImpl *button, Display *display, Potentiometer *pot, ServoMotorImpl *servo)
{
    this->button = button;
    this->display = display;
    this->pot = pot;
    this->servo = servo;

    windowLevel = 0;
    dashboardValue = 0;
    potValue = 0;
    temperature = 0.0;

    display->init();
    display->turnOnDisplay();
    display->defaultMessage();
    servo->on();
    servo->setPosition(CLOSED_POS);
    notifyAutomatic();

    dashboardValueChanged = false;
    windowLevelChanged = false;
    modeChanged = true;
    alarm = false;
}

void Window::changeMode(Window::Mode newMode)
{
    modeChanged = mode != newMode;
    mode = newMode;
}

void Window::notifyAutomatic()
{
    display->updateMode("AUTOMATIC");
    changeMode(AUTOMATIC);
}

void Window::notifyManual()
{
    display->updateMode("MANUAL");
    changeMode(MANUAL);
}

void Window::notifyAlarmResetted()
{
    alarm = false;
}

void Window::notifyAlarm() {
    alarm = true;
}


bool Window::readButton()
{
    return button->isPressed();
}

int Window::getPotValue()
{
    return potValue;
}

bool Window::didDashboardValueChanged()
{
    return dashboardValueChanged;
}

void Window::updatePotValue()
{
    potValue = pot->detectValue();
}

void Window::moveWindow(int pos)
{
    servo->setPosition(pos);
    windowLevel = pos;
    char windowBuffer[4];
    windowLevelChanged = true;
    dashboardValueChanged = false;

    display->updateLevel(itoa(windowLevel, windowBuffer, 10));
}

float Window::getTemp()
{
    return temperature;
}

bool Window::didWindowLevelChanged()
{
    return windowLevelChanged;
}

bool Window::didModeChanged()
{
    return modeChanged;
}

void Window::notifySending(int sended)
{
    if (sended == SENDED_MODE) {
        modeChanged = false;
    } else if (sended == SENDED_POS) {
        windowLevelChanged = false;
    }
}

int Window::getWindowLevel()
{
    return windowLevel;
}
void Window::updateDashboardValue(int value)
{
    dashboardValue = value;
    dashboardValueChanged = true;
}

int Window::getDashboardValue()
{
    return dashboardValue;
}

void Window::updateTemp(float temp)
{
    char charTemp[5];
    temperature = temp;
    display->updateTemperature(dtostrf(temp, 3, 1, charTemp));
}

bool Window::isAuto() {
    return mode == Window::AUTOMATIC && !alarm;
}

bool Window::isManual() {
    return mode == Window::MANUAL && !alarm;
}
