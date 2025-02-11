#ifndef __TASKSENDINGMESSAGE__
#define __TASKSENDINGMESSAGE__

#include "tasks/Task.h"
#include "window/Window.h"
#include "kernel/MsgService.h"

class TaskSendingMessage : public Task
{

public:
    TaskSendingMessage(Window *window);
    void tick();

private:
    enum SendingState
    {
        WAIT,
        SENDING
    } state;

    Window *window;
};

#endif