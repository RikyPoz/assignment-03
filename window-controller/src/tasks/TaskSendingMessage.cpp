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
        int lvl = window->getWindowLevel();
        String mode = window->getState();
        MsgService.sendMsg(String(lvl)+","+String(mode));
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
