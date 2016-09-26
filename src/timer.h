#ifndef TIMER_H

#include "common.h"

typedef struct {
    uint32_t startTime;
    uint32_t currentTime;
    uint32_t previousTime;
    uint32_t previousFpsUpdate;
    
    float fps;
    int frames;
    float delta;

} Timer;

void Timer_Init(Timer *t);
void Timer_Step(Timer *t);
void Timer_Sleep(uint32_t ms);
float Timer_Elapsed(Timer *t);

#endif // TIMER_H
