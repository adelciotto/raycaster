#ifndef TIMER_H
#define TIMER_H

#include "common.h"

class Graphics;

class ProfileTimer {
    std::string name;
    uint64_t prev;
    uint64_t now;
    uint64_t freq;
    float delta;

public:
    ProfileTimer(const std::string& name);

    float start();
    float end();
    int printDelta(Graphics& graphics, int x, int y, uint32_t color, uint32_t bgColor);
};

#endif // TIMER_H
