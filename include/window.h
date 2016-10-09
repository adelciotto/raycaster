#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"

enum WindowModes {
    fullscreen = SDL_WINDOW_FULLSCREEN,
    fullscreenDesktop = SDL_WINDOW_FULLSCREEN_DESKTOP,
    windowed = 0
};

typedef std::unique_ptr<SDL_Window, void(*)(SDL_Window *)> SDLWindowPtr;

class Window {
public:
    Window();
    Window(int width, int height, bool fullscreen);

    SDL_Window *getSDLWindow() const { return sdlWindow.get(); }
    void create(int w, int h, bool fullscreen);
    void setSize(int width, int height);
    void setMode(WindowModes mode);

private:
    int width;
    int height;
    SDLWindowPtr sdlWindow;
};

#endif // WINDOW_H
