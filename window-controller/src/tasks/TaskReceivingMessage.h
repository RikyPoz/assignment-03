#ifndef __TASKRECEIVEMESSAGE__
#define __TASKRECEIVEMESSAGE__

#include "tasks/Task.h"
#include "window/Window.h"

class TaskReceivingMessage : public Task
{

public:
    TaskReceivingMessage(Window *window);
    void tick();

private:
    Window *window;
};

#endif