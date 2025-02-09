#ifndef __MODALITYTASK__
#define __MODALITYTASK__

#include "tasks/Task.h"
#include "window/Window.h"

class ModalityTask : public Task
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
