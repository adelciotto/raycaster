#include "window.h"

const WindowMode Fullscreen = SDL_WINDOW_FULLSCREEN;
const WindowMode FullscreenDesktop = SDL_WINDOW_FULLSCREEN_DESKTOP; 
const WindowMode Windowed = 0;

void Window_Init(Window *w, int width, int height, bool fullscreen) {
    uint32_t flags = SDL_WINDOW_RESIZABLE;

    flags |= fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    SDL_Window *win = SDL_CreateWindow(
            "wolf3d", 
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, 
            width, 
            height,
            flags
    );

    if (win == NULL) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    w->width = width;
    w->height = height;
    w->sdlWindow = win;
}

void Window_Destroy(Window *w) {
    SDL_DestroyWindow(w->sdlWindow);
}

void Window_SetSize(int width, int height) {
    
}

void Window_SetMode(Window *w, WindowMode mode) {
    if (SDL_SetWindowFullscreen(w->sdlWindow, mode) < 0) {
        printf("Warning: Could not set fullscreen mode: %s\n", SDL_GetError());
    }
}
