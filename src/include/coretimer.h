#ifndef CORETIMER_H
#define CORETIMER_H

#include "common.h"

class CoreTimer {
    uint32_t startTime;
    uint32_t currentTime;
    uint32_t previousTime;
    uint32_t previousFpsUpdate;
    
    float fps;
    int frames;
    float delta;

public:
    CoreTimer();

    float ticks() const { return (float)SDL_GetTicks(); }
    float getFPS() const { return fps; }
    float getDelta() const { return delta; }
    float elapsed() const { return float(SDL_GetTicks() - startTime); }
    void step();
    void sleep(uint32_t ms);
};

#endif // CORETIMER_H
