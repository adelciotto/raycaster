#ifndef TIMER_H
#define TIMER_H

#include "common.h"

class Timer {
    uint32_t startTime;
    uint32_t currentTime;
    uint32_t previousTime;
    uint32_t previousFpsUpdate;
    
    float fps;
    int frames;
    float delta;

public:
    Timer();

    float getFPS() const { return fps; }
    float getDelta() const { return delta; }
    float elapsed() const { return float(SDL_GetTicks() - startTime) / 1000.0f; }
    void step();
    void sleep(uint32_t ms);
};

#endif // TIMER_H
