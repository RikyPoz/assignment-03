#ifndef __TASKSENDING__
#define __TASKSENDING__

#include "tasks/TaskWithTimer.h"
#include "Window/Window.h"

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
        SENDING,
        WAIT,
        TRASM_TIME

    } state;
};

#endif
