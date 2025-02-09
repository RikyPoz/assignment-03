#ifndef __TASKSENDINGMESSAGE__
#define __TASKSENDINGMESSAGE__

#include "tasks/TaskWithTimer.h"
#include "window/Window.h"

class TaskSendingMessage : public TaskWithTimer
{

public:
    TaskSendingMessage(Window *window);
    void tick();

private:
    enum State
    {
        WAIT,
        SENDING
    } state;

    Window *window;
};

#endif