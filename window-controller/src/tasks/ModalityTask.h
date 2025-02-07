#ifndef __MODALITYTASK__
#define __MODALITYTASK__

#include "tasks/TaskWithTimer.h"
#include "Window/Window.h"

class ModalityTask : public TaskWithTimer
{
public:
    ModalityTask(Window *window);
    void init(int period);
    void tick();

private:
    Window *window;
    enum DetectionState
    {
        AUTOMATIC,
        MANUAL
    } state;
};

#endif
