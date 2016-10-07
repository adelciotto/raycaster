#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include "common.h"

class Timer {
    uint64_t prev;
    uint64_t now;
    uint64_t freq;
    float delta;
    
    uint32_t previousIntervalTime;

public:
    Timer() 
        : now(SDL_GetPerformanceCounter()),
          prev(SDL_GetPerformanceCounter()),
          freq(SDL_GetPerformanceFrequency()),
          delta(0.0f) { }

    float start() {
        prev = now;
        now = SDL_GetPerformanceCounter();
        delta = float(now - prev) / float(freq);

        return delta;
    }

    float end() {
        now = SDL_GetPerformanceCounter();
        delta = float((now - prev)*1000) / float(freq);

        return delta;
    }

    void setInterval(uint32_t interval, const std::function<void()>&& fn) {
        uint32_t now = SDL_GetTicks();
        uint32_t intervalDelta = now - previousIntervalTime;

        if (intervalDelta > interval) {
            fn();
            previousIntervalTime = now;
        }
    }
};

#endif // TIMER_H
