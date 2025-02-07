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
        if (window->readButton())
        {
            state = MANUAL;
            window->notifyManual();
            resetTimer();
        }

        window->updateWindowLevel(random(1, 100)); // da togliere è per simulare

        break;

    case MANUAL:
        if (window->readButton())
        {
            state = AUTOMATIC;
            window->notifyAutomatic();
        }

        break;
    }
}
