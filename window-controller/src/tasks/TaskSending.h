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
    int oldPos;
    bool IsOldModeManual;
    enum SendingState
    {
        WAIT,
        TRASM_TIME

    } state;
};

#endif
