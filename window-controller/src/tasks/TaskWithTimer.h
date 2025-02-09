#ifndef __TASKWITHTIMER__
#define __TASKWITHTIMER__

#include "Task.h"

class TaskWithTimer : public Task {
public:
    void init(int period) override {
        Task::init(period);
        t = 0;
    }

    bool updateAndCheckTime(int basePeriod) override {
        timeElapsed += basePeriod;
        if (timeElapsed >= myPeriod) {
            t += timeElapsed;
            timeElapsed = 0;
            return true;
        }
        return false;
    }

protected:
    long getElapsedTime() {
        return t;
    }

    void resetTimer() {
        t = 0;
    }

private:
    long t;

};

#endif
