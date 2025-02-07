#ifndef __WINDOWCONTROLTASK__
#define __WINDOWCONTROLTASK__

#include "tasks/TaskWithTimer.h"
#include "Window/Window.h"

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
        UNIT_CONTROL,
        POT_CONTROL
    } state;
};

#endif
