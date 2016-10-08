#include "profile_timer.h"
#include "graphics.h"

ProfileTimer::ProfileTimer(const std::string& name)
    : name(name),
      prev(SDL_GetPerformanceCounter()),
      now(SDL_GetPerformanceCounter()),
      freq(SDL_GetPerformanceFrequency()),
      delta(0.0f) { }

float ProfileTimer::start() {
    prev = now;
    now = SDL_GetPerformanceCounter();
    delta = float(now - prev) / float(freq);

    return delta;
}

float ProfileTimer::end() {
    now = SDL_GetPerformanceCounter();
    delta = float((now - prev)*1000) / float(freq);

    return delta;
}

int ProfileTimer::printDelta(Graphics& graphics, int x, int y, uint32_t color, uint32_t bgColor) {
    const std::string text = utils::stringFormat("%s: %.2f", name.c_str(), delta);

    return graphics.drawString(text, x, y, color, true, bgColor);
}
