#include "tasks/TaskSendingMessage.h"
#include <Arduino.h>
#include "const.h"
#include "kernel/MsgService.h"

#define SENDING_TIME 100

TaskSendingMessage::TaskSendingMessage(Window *window)
{
    state = SENDING;
}

void TaskSendingMessage::tick()
{
    switch (state)
    {
    case SENDING:
    {
        String modality;
        int lvl = window->getWindowLevel();
        Window::State mode = window->getState();
        if (mode == 0)
        {
            modality = "AUTOMATIC";
        }
        else if (mode == 1)
        {
            modality = "MANUAL";
        }
        MsgService.sendMsg(String(lvl) + "," + modality);
        resetTimer();
        state = WAIT;
        break;
    }
    case WAIT:
    {
        if (getElapsedTime() > SENDING_TIME)
        {
            state = SENDING;
        }
        break;
    }
    }
}
