#include "WindowControlTask.h"
#include "Arduino.h"
#include "const.h"
#define TOLERANCE 2

WindowControlTask::WindowControlTask(Window *window)
{
    this->window = window;
    oldUnitValue = -1;
    oldPotValue = -1;
}

void WindowControlTask::init(int period)
{
    Task::init(period);
    state = AUTOMATIC;
    
}

void WindowControlTask::tick()
{
    switch (state)
    {
    case AUTOMATIC:
        if (window->isAuto()) {
            int newUnitValue = window->getDashboardValue();
            if (newUnitValue != oldUnitValue) {
                window->moveWindow(newUnitValue);
                oldUnitValue = newUnitValue;
            }
        } else {
            state = MANUAL;
        }

        break;

    case MANUAL:
        if (window->isManual()) {
            int newUnitValue = window->getDashboardValue();
            int newPotValue = window->getPotValue();

            if (abs(oldPotValue - newPotValue) > TOLERANCE) {
                window->moveWindow(newPotValue);
            } else if (oldUnitValue != newUnitValue) {
                window->moveWindow(newUnitValue);
            }
            oldPotValue = newPotValue;
            oldUnitValue = newUnitValue;
        } else {
            state = AUTOMATIC;
        }

        break;
    }
}
