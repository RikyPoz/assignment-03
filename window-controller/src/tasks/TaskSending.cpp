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
        case WAIT:
        {
            if (window->didModeChanged()) {
                state = SENDING_MODE;
            } else if (window->didWindowLevelChanged() && window->isManual()) {
                state = SENDING_POS;
            }
            break;
        }
        case SENDING_MODE:
        {
            String modality;
            modality = window->isAuto() ? "AUTOMATIC" : "MANUAL";
            MsgService.sendMsg("mode_"+modality);
            window->notifySending(SENDED_MODE);
            resetTimer();
            state = TRASM_TIME;
            break;
        }
        case SENDING_POS:
        {
            int pos = window->getWindowLevel();
            MsgService.sendMsg("position_"+String(pos));
            window->notifySending(SENDED_POS);
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
    }
}
