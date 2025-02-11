#include "tasks/TaskSendingMessage.h"
#include <Arduino.h>
#include "const.h"
#include "kernel/MsgService.h"

#define SENDING_TIME 50

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
        delay(10);
        String modality;
        int lvl = window->getWindowLevel();
        Window::State mode = window->getState();
        delay(10);

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
        delay(10);
        Serial.println("[TASK] lvl:" + String(lvl));
        // resetTimer();
        state = WAIT;
        break;
    }
    case WAIT:
    {

        break;
    }
    }
}
