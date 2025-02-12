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
}

Window::State Window::getState()
{
    return state;
}

void Window::changeState(Window::State newState)
{
    state = newState;
    modeChanged = true;
}

void Window::notifyAutomatic()
{
    display->updateMode("AUTOMATIC");
    changeState(AUTOMATIC);
}

void Window::notifyManual()
{
    display->updateMode("MANUAL   ");
    changeState(MANUAL);
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
    int newPotValue = pot->detectValue();
    potValue = newPotValue != potValue ? newPotValue : potValue;
}

void Window::moveWindow(int pos)
{
    servo->setPosition(pos);
    windowLevel = pos;
    windowLevelChanged = true;
    dashboardValueChanged = false; // qui o da cambiare?

    display->updateLevel(String(windowLevel));
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
    temperature = temp;
    display->updateTemperature(String(temperature));
}

bool Window::isAuto()
{
    return state == Window::AUTOMATIC;
}
