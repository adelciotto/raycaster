#ifndef WINDOW_H

#include "common.h"

typedef struct window_t {
    int width;
    int height;
    SDL_Window *sdlWindow;
} Window;

typedef uint32_t WindowMode;

extern const WindowMode Fullscreen;
extern const WindowMode FullscreenDesktop;
extern const WindowMode Windowed;

void Window_Init(Window *w, int width, int height, bool fullscreen);
void Window_Destroy(Window *window);
void Window_SetSize(int width, int height);
void Window_SetMode(Window *w, WindowMode mode);

#endif // WINDOW_H
