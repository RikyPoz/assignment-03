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
            int pos = window->getWindowLevel();
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
            int pos = window->getPotValue();
            window->readPotValue();
            int newPos = window->getPotValue();
            if (pos != newPos)
            {
                window->updateWindowLevel(newPos);
                window->moveWindow(newPos);
            }
        }
        else
        {
            state = AUTOMATIC;
        }

        break;
    }
}
