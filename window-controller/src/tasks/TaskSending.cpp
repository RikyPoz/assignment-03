#include "TaskSending.h"
#include "Arduino.h"
#include "const.h"
#include "kernel/MsgService.h"

#define SENDING_TIME 250

TaskSending::TaskSending(Window *window)
{
    this->window = window;
    oldPos = -1;
    IsOldModeManual;
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
            int newPos = window->getWindowLevel();
            bool IsNewModeManual = window->isManual();
            bool isModeChanged = IsOldModeManual != IsNewModeManual;
            if (isModeChanged && !window->isAlarm()) {
                String mode = IsNewModeManual ? "MANUAL" : "AUTOMATIC";
                MsgService.sendMsg("mode_"+mode);
                IsOldModeManual = IsNewModeManual;
                state = TRASM_TIME;
            } else if (newPos != oldPos && IsNewModeManual) {
                MsgService.sendMsg("position_"+String(newPos));
                oldPos = newPos;
                state = TRASM_TIME;
            }
            resetTimer();
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
