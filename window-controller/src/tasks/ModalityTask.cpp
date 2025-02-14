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
    state = MONITORING;
}

void ModalityTask::tick()
{
    switch (state)
    {
    case MONITORING:
        if (window->readButton()) {
            if (window->isAuto()) {
                window->notifyManual();
            } else if (window->isManual()) {
                window->notifyAutomatic();
            }
            state = WAIT;
            resetTimer();
        }
        break;
    case WAIT:
    {
        if (getElapsedTime() > BOUNCING_TIME) {
            state = MONITORING;
        }
        break;
    }
    }
}
