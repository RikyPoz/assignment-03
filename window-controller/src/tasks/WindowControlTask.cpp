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
    state = UNIT_CONTROL;
}

void WindowControlTask::tick()
{
    switch (state)
    {
    case UNIT_CONTROL:
        if (window->getState() == Window::AUTOMATIC)
        {
            int pos = window->getWindowLevel();
            window->moveWindow(pos);
        }
        else
        {
            state = POT_CONTROL;
        }

        break;

    case POT_CONTROL:
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
            state = UNIT_CONTROL;
        }

        break;
    }
}
