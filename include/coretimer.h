#ifndef CORETIMER_H
#define CORETIMER_H

#include "common.h"

class CoreTimer {
public:
    CoreTimer();

    double ticks() const;
    double getFPS() const;
    double getDelta() const;
    bool isPaused() const;
    uint32_t elapsed() const;
    void start();
    void pause();
    void resume();
    void step();
    void delay();
    void sleep(uint32_t ms);

private:
    static bool instantiated;

    uint32_t startTime;
    uint32_t fpsLimitStartTime;
    uint32_t pausedTime;
    uint32_t previousTime;
    uint32_t previousFpsUpdate;

    double fps;
    int frames;
    double delta;
    bool paused;
};

#endif // CORETIMER_H
