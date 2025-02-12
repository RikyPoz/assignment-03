#ifndef __WINDOWCONTROLTASK__
#define __WINDOWCONTROLTASK__

#include "tasks/TaskWithTimer.h"
#include "window/Window.h"

class WindowControlTask : public TaskWithTimer
{
public:
    WindowControlTask(Window *window);
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
