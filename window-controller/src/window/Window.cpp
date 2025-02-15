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
    modeChanged = true;
    alarm = false;
}

void Window::changeMode(Window::Mode newMode)
{
    if (mode != newMode) {
        modeChanged = true;
        mode = newMode;
    }
}

void Window::notifyAutomatic()
{
    display->updateMode("AUTOMATIC");
    display->noTemperature();
    changeMode(AUTOMATIC);
}

void Window::notifyManual()
{
    display->updateMode("MANUAL");
    display->updateTemperature(temperature);
    changeMode(MANUAL);
}

void Window::notifyAlarmResetted()
{
    alarm = false;
}

void Window::notifyAlarm() {
    alarm = true;
}

bool Window::isAlarm() {
    return alarm;
}


bool Window::readButton()
{
    return button->isPressed();
}

int Window::getPotValue()
{
    return pot->detectValue();
}

void Window::moveWindow(int pos)
{
    servo->setPosition(pos);
    windowLevel = pos;
    dashboardValueChanged = false;

    display->updateLevel(pos);
}

float Window::getTemp()
{
    return temperature;
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
    if (temperature != temp && isManual()) {
        display->updateTemperature(temp);
    }
    temperature = temp;
}

bool Window::isAuto() {
    return mode == Window::AUTOMATIC && !alarm;
}

bool Window::isManual() {
    return mode == Window::MANUAL && !alarm;
}
