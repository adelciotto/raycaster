#include "coretimer.h"

namespace {
    const double fpsUpdateFreq = 1.0;
    const double fps = 60.0;
    const double timePerFrame = 1000.0 / fps;
}

bool CoreTimer::instantiated = false;

CoreTimer::CoreTimer() 
    : startTime(SDL_GetTicks()),
      fpsLimitStartTime(SDL_GetTicks()),
      previousTime(SDL_GetTicks()),
      previousFpsUpdate(SDL_GetTicks()),
      fps(0.0),
      frames(0),
      delta(0.0),
      paused(false) { 
    assert(!instantiated);
    instantiated = true;
}

double CoreTimer::ticks() const {
    return double(SDL_GetTicks());
}

double CoreTimer::getFPS() const {
    return fps;
}

double CoreTimer::getDelta() const {
    return delta / 1000.0;
}

bool CoreTimer::isPaused() const {
    return paused;
}

uint32_t CoreTimer::elapsed() const {
    if (paused) {
        return pausedTime;
    } else {
        return SDL_GetTicks() - startTime;
    }
}

void CoreTimer::start() {
    fpsLimitStartTime = SDL_GetTicks();
}

void CoreTimer::pause() {
    if (!paused) {
        paused = true;
        pausedTime = SDL_GetTicks() - startTime;
        startTime = 0;
    }
}

void CoreTimer::resume() {
    if (paused) {
        paused = false;
        startTime = SDL_GetTicks() - pausedTime;
        pausedTime = 0;
    }
}

void CoreTimer::step() {
    if (!paused) frames++;

    // Calculate the delta-time (in seconds) between now and the last call to Timer.step.
    uint32_t currentTime = elapsed();
    delta = double(currentTime - previousTime);
    previousTime = currentTime;

    double lastFpsUpdateTime = double(currentTime - previousFpsUpdate) / 1000.0;

    // Update the FPS once every second to avoid a jittery counter.
    if (lastFpsUpdateTime > fpsUpdateFreq) {
        fps = double(frames / fpsUpdateFreq);
        previousFpsUpdate = currentTime;
        frames = 0;
    }
}

void CoreTimer::delay() {
    uint32_t totalFrameTime = SDL_GetTicks() - fpsLimitStartTime;

    if (totalFrameTime < timePerFrame) {
        SDL_Delay(uint32_t(timePerFrame - totalFrameTime));
    }
}

void CoreTimer::sleep(uint32_t ms) {
    if (ms > 0) {
        SDL_Delay(ms);
    }
}
