#include "TaskSending.h"
#include "Arduino.h"
#include "const.h"
#include "kernel/MsgService.h"

#define SENDING_TIME 250

TaskSending::TaskSending(Window *window)
{
    this->window = window;
}

void TaskSending::init(int period)
{
    Task::init(period);
    state = WAIT;
}

void TaskSending::tick()
{
    switch (state)
    {
    case SENDING:
    {
        String modality;
        int lvl = window->getWindowLevel();
        Window::State mode = window->getState();

        if (mode == Window::AUTOMATIC)
        {
            modality = "AUTOMATIC";
        }
        else if (mode == Window::MANUAL)
        {
            modality = "MANUAL";
        }
        else
        {
            modality = "prova";
        }
        String msg = String(lvl) + "," + modality;
        MsgService.sendMsg(msg);
        window->notifySending();
        resetTimer();
        state = TRASM_TIME;
        break;
    }
    case TRASM_TIME:
    {
        if (getElapsedTime() >= SENDING_TIME)
        {
            state = WAIT;
        }
        break;
    }
    case WAIT:
    {
        if (window->didWindowLevelChanged() || window->didModeChanged())
        {
            state = SENDING;
        }
    }
    }
}
