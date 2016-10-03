#ifndef TIMER_H
#define TIMER_H

#include "common.h"

class Timer {
    uint64_t prev;
    uint64_t now;
    uint64_t freq;
    float delta;
    
    uint32_t previousPrintTime;

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

    template<typename... Args>
    void print(int interval, const std::string& format, Args... args) {
        uint32_t now = SDL_GetTicks();
        uint32_t printDelta = now - previousPrintTime;

        if (printDelta > interval) {
            printf(format.c_str(), args ...);
            previousPrintTime = now;
        }
    }
};

#endif // TIMER_H
