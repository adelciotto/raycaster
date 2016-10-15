#include "profile_timer.h"
#include "graphics.h"

ProfileTimer::ProfileTimer(const std::string& name)
    : name(name),
      prev(SDL_GetPerformanceCounter()),
      now(SDL_GetPerformanceCounter()),
      freq(SDL_GetPerformanceFrequency()),
      delta(0.0) { }

double ProfileTimer::start() {
    prev = now;
    now = SDL_GetPerformanceCounter();
    delta = double(now - prev) / double(freq);

    return delta;
}

double ProfileTimer::end() {
    now = SDL_GetPerformanceCounter();
    delta = double((now - prev)*1000) / double(freq);

    return delta;
}

int ProfileTimer::printDelta(Graphics& graphics, int x, int y, uint32_t color, uint32_t bgColor) {
    const std::string text = utils::stringFormat("%s: %.2f", name.c_str(), delta);

    return graphics.drawString(text, x, y, color, true, bgColor);
}
