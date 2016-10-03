#include "coretimer.h"

const float fpsUpdateFreq = 1.0f;

CoreTimer::CoreTimer() 
    : startTime(SDL_GetTicks()),
      currentTime(0),
      previousTime(SDL_GetTicks()),
      previousFpsUpdate(SDL_GetTicks()),
      fps(0.0f),
      frames(0),
      delta(0.0f) { }

void CoreTimer::step() {
    frames++;

    // Calculate the delta-time (in seconds) between now and the last call to Timer.step.
    previousTime = currentTime;
    currentTime = SDL_GetTicks();
    delta = float(currentTime - previousTime) / 1000.0f;

    float lastFpsUpdateTime = float(currentTime - previousFpsUpdate) / 1000.0f;

    // Update the FPS once every second to avoid a jittery counter.
    if (lastFpsUpdateTime > fpsUpdateFreq) {
        fps = float(frames / fpsUpdateFreq);
        previousFpsUpdate = currentTime;
        frames = 0;

        //printf("FPS: %.2f\n", fps);
    }
}

void CoreTimer::sleep(uint32_t ms) {
    if (ms > 0) {
        SDL_Delay(ms);
    }
}
