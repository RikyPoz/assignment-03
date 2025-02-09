#include "ModalityTask.h"
#include "Arduino.h"
#include "const.h"

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
        if (window->isAuto()) {
            if (window->readButton())
            {
                state = MANUAL;
                window->notifyManual();
            }

            window->updateWindowLevel(random(1, 100)); // da togliere è per simulare
        } else {
            state = MANUAL;
        }
        break;

    case MANUAL:
        if (!window->isAuto()) {
            if (window->readButton())
            {
                state = AUTOMATIC;
                window->notifyAutomatic();
            }
        } else {
            state = AUTOMATIC;
        }
        break;
    }
}
