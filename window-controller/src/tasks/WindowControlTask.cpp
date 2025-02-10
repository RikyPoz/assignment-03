#include "WindowControlTask.h"
#include "Arduino.h"
#include "const.h"

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
            int pos = window->getDashboardValue();
            window->moveWindow(pos);
        }
        else
        {
            state = MANUAL;
        }

        break;

    case MANUAL:
        if (window->getState() == Window::MANUAL)
        {
            int windowLevel = window->getWindowLevel();
            int dashboardValue = window->getDashboardValue();
            window->updatePotValue();
            int potValue = window->getPotValue();

            if (windowLevel != potValue)
            {
                window->updateWindowLevel(potValue);
                window->moveWindow(potValue);
            }
            else if (windowLevel != dashboardValue)
            {
                window->updateWindowLevel(dashboardValue);
                window->moveWindow(dashboardValue);
            }
        }
        else
        {
            state = AUTOMATIC;
        }

        break;
    }
}
