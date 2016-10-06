#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"

enum WindowModes {
    fullscreen = SDL_WINDOW_FULLSCREEN,
    fullscreenDesktop = SDL_WINDOW_FULLSCREEN_DESKTOP,
    windowed = 0
};

class Window {
    int width;
    int height;
    std::unique_ptr<SDL_Window, void(*)(SDL_Window *)> sdlWindow;

public:
    Window(int width, int height, bool fullscreen);

    SDL_Window *getSDLWindow() const { return sdlWindow.get(); }
    void setSize(int width, int height);
    void setMode(WindowModes mode);
};

#endif // WINDOW_H
