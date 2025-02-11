#ifndef __MODALITYTASK__
#define __MODALITYTASK__

#include "tasks/TaskWithTimer.h"
#include "window/Window.h"

class ModalityTask : public TaskWithTimer
{
public:
    ModalityTask(Window *window);
    void init(int period);
    void tick();

private:
    Window *window;
    enum State
    {
        AUTOMATIC,
        MANUAL,
        WAIT
    } state;

    State precState;
};

#endif
