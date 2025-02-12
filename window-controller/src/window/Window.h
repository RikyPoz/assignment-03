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
    enum Mode
    {
        AUTOMATIC,
        MANUAL
    };

    Window(ButtonImpl *button, Display *display, Potentiometer *pot, ServoMotorImpl *servo);

    // Metodi di gestione dello stato
    Mode getMode();
    bool isAuto();

    // Interrogazioni sensori
    bool readButton();
    void moveWindow(int angle);
    void updateTemp(float temp);
    float getTemp();

    bool didWindowLevelChanged();
    bool didModeChanged();
    void notifySending(int sended);

    int getWindowLevel();
    void updateDashboardValue(int value);
    int getDashboardValue();
    void updatePotValue();
    int getPotValue();
    bool didDashboardValueChanged();

    // Eventi per cambiare stato
    void notifyAutomatic();
    void notifyManual();

private:
    void changeMode(Mode newMode);
    Mode mode; // Stato attuale della FSM
    Display *display;
    ServoMotor *servo;
    ButtonImpl *button;
    Potentiometer *pot;
    float temperature; // solo da visualizzare
    int windowLevel;
    int potValue;
    int dashboardValue;
    bool dashboardValueChanged;
    bool windowLevelChanged;
    bool modeChanged;
};

#endif
