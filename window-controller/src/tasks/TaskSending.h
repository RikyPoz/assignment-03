#ifndef __TASKSENDING__
#define __TASKSENDING__

#include "tasks/TaskWithTimer.h"
#include "window/Window.h"

class TaskSending : public TaskWithTimer
{
public:
    TaskSending(Window *window);
    void init(int period);
    void tick();

private:
    Window *window;
    enum SendingState
    {
        SENDING_POS,
        SENDING_MODE,
        WAIT,
        TRASM_TIME

    } state;
};

#endif
