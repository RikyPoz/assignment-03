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
    bool isAuto();

    // Interrogazioni sensori
    bool readButton();
    void moveWindow(int angle);
    void updateTemp(String temp);

    void updateWindowLevel(int level);
    int getWindowLevel();
    void updateDashboardValue(int value);
    int getDashboardValue();
    void updatePotValue();
    int getPotValue();

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
    String temperature; // solo da visualizzare
    int windowLevel;
    int potValue;
    int dashboardValue;
};

#endif
