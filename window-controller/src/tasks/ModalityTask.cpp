#include "ModalityTask.h"
#include "Arduino.h"
#include "const.h"

#define BOUNCING_TIME 150

ModalityTask::ModalityTask(Window *window)
{
    this->window = window;
}

void ModalityTask::init(int period)
{
    Task::init(period);
    state = AUTOMATIC;
}

void ModalityTask::tick()
{
    switch (state)
    {
    case AUTOMATIC:
        if (window->isAuto())
        {
            if (window->readButton())
            {
                state = WAIT;
                precState = AUTOMATIC;
                window->notifyManual();
                resetTimer();
            }
        }
        else
        {
            state = MANUAL;
        }
        break;

    case MANUAL:
        if (!window->isAuto())
        {
            if (window->readButton())
            {
                state = WAIT;
                precState = MANUAL;
                window->notifyAutomatic();
                resetTimer();
            }
        }
        else
        {
            state = AUTOMATIC;
        }
        break;
    case WAIT:
    {
        if (getElapsedTime() > BOUNCING_TIME)
        {
            state = precState == AUTOMATIC ? MANUAL : AUTOMATIC;
        }
        break;
    }
    }
}
