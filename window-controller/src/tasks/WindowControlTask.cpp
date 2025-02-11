#include "WindowControlTask.h"
#include "Arduino.h"
#include "const.h"
#define TOLERANCE 2

WindowControlTask::WindowControlTask(Window *window)
{
    this->window = window;
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
        if (window->getState() == Window::AUTOMATIC)
        {
            int dashboardPos = window->getDashboardValue();
            if (window->didDashboardValueChanged())
            {
                window->moveWindow(dashboardPos);
            }
        }
        else
        {
            state = MANUAL;
        }

        break;

    case MANUAL:
        if (window->getState() == Window::MANUAL)
        {
            int dashboardValue = window->getDashboardValue();

            int potValue = window->getPotValue();
            window->updatePotValue();
            int newPotValue = window->getPotValue();

            if (abs(potValue - newPotValue) > TOLERANCE)
            {
                window->moveWindow(newPotValue);
            }
            else if (window->didDashboardValueChanged())
            {
                window->moveWindow(dashboardValue);
                // dashboardValueChanged = false qui o in moveWindow per entrambi?
            }
        }
        else
        {
            state = AUTOMATIC;
        }

        break;
    }
}
