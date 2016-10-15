#ifndef TIMER_H
#define TIMER_H

#include "common.h"

class Graphics;

class ProfileTimer {
public:
    ProfileTimer(const std::string& name);

    double start();
    double end();
    int printDelta(Graphics& graphics, int x, int y, uint32_t color, uint32_t bgColor);

private:
    std::string name;
    uint64_t prev;
    uint64_t now;
    uint64_t freq;
    double delta;

};

#endif // TIMER_H
