#include "timer.h"

const float fpsUpdateFreq = 1.0f;

void Timer_Init(Timer *t) {
    t->startTime = t->previousTime = SDL_GetTicks();
    t->currentTime = 0;
    t->previousFpsUpdate = SDL_GetTicks();

    t->fps = 0.0f;
    t->frames = 0;
    t->delta = 0.0f;
}

void Timer_Step(Timer *t) {
    t->frames++;

    // Calculate the delta-time (in seconds) between now and the last call to Timer_Step.
    t->previousTime = t->currentTime;
    t->currentTime = SDL_GetTicks();
    t->delta = (float)(t->currentTime - t->previousTime) / 1000.0f;

    float lastFpsUpdateTime = (float)(t->currentTime - t->previousFpsUpdate) / 1000.0f;

    // Update the FPS once every second to avoid a jittery counter.
    if (lastFpsUpdateTime > fpsUpdateFreq) {
        t->fps = (float)t->frames / fpsUpdateFreq;
        t->previousFpsUpdate = t->currentTime;
        t->frames = 0;

        printf("FPS: %.2f\n", t->fps);
    }
}

void Timer_Sleep(uint32_t ms) {
    if (ms > 0) {
        SDL_Delay(ms);
    }
}

float Timer_Elapsed(Timer *t) {
    return (float)(SDL_GetTicks() - t->startTime) / 1000.0f;
}
