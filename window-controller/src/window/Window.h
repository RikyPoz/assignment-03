#ifndef __WINDOW__
#define __WINDOW__
#include <Arduino.h>
#include "devices/ButtonImpl.h"
#include "kernel/MsgService.h"
#include "devices/Display.h"
#include "devices/Potentiometer.h"
#include "devices/Servo_motor_impl.h"

class Window
{
public:
    enum State
    {
        AUTOMATIC,
        MANUAL
    };

    Window(ButtonImpl *button, Display *display, Potentiometer *pot, ServoMotorImpl *servo);

    // Metodi di gestione dello stato
    State getState();
    bool isStateChanged();

    // Interrogazioni sensori
    bool readButton();
    int getPotValue();
    void readPotValue();
    void moveWindow(int angle);
    int getWindowLevel();
    void updateTemp(String temp);
    void updateWindowLevel(int level);

    // Eventi per cambiare stato
    void notifyAutomatic();
    void notifyManual();

private:
    void changeState(State newState);
    State state; // Stato attuale della FSM
    bool stateChanged;
    Display *display;
    ServoMotor *servo;
    ButtonImpl *button;
    Potentiometer *pot;
    int windowLevel;
    String temperature; // solo da visualizzare
    int potValue;
};

#endif
