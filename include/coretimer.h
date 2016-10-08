#ifndef CORETIMER_H
#define CORETIMER_H

#include "common.h"

class CoreTimer {
    static bool instantiated;

    uint32_t startTime;
    uint32_t fpsLimitStartTime;
    uint32_t pausedTime;
    uint32_t previousTime;
    uint32_t previousFpsUpdate;
    
    float fps;
    int frames;
    float delta;
    bool paused;

public:
    CoreTimer();

    float ticks() const;
    float getFPS() const;
    float getDelta() const;
    bool isPaused() const;
    uint32_t elapsed() const;
    void start();
    void pause();
    void resume();
    void step();
    void delay();
    void sleep(uint32_t ms);
};

#endif // CORETIMER_H
