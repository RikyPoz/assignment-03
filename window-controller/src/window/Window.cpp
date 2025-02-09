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

    changeState(AUTOMATIC);
    display->init();
    display->turnOnDisplay();
    display->automaticMessage();
    servo->on();
    servo->setPosition(CLOSED_POS);
    windowLevel = 0;
    temperature = "0";
}

Window::State Window::getState()
{
    return state;
}

void Window::changeState(Window::State newState)
{
    state = newState;
    stateChanged = true;
}

bool Window::isStateChanged()
{
    if (stateChanged)
    {
        stateChanged = false;
        return true;
    }
    return false;
}

void Window::notifyAutomatic()
{
    display->automaticMessage();
    changeState(AUTOMATIC);
}

void Window::notifyManual()
{
    display->manualMessage(temperature);
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

void Window::readPotValue()
{
    potValue = pot->detectValue();
}

void Window::moveWindow(int angle)
{
    servo->setPosition(angle);
}

int Window::getWindowLevel()
{
    return windowLevel;
}
void Window::updateWindowLevel(int level)
{
    windowLevel = level;
    Serial.println("WindowLevel update:" + String(getWindowLevel()));
}

void Window::updateTemp(String temp)
{
    temperature = temp;
    display->manualMessage(temperature);
}

bool Window::isAuto() {
    return state == Window::AUTOMATIC;
}
