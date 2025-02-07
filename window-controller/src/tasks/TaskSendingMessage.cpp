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
        Serial.println("Window Level before sending: " + String(lvl));
        // MsgService.sendMsg("WindowLevel:" + String(lvl));
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
