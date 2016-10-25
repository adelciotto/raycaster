#include <stdexcept>
#include "window.h"

namespace {
    SDL_Window *createSDLWindow(int width, int height, bool fullscreen);
    void destroySDLWindow(SDL_Window *win);
}

Window::Window()
    : width(640),
      height(480),
      sdlWindow(nullptr, destroySDLWindow) { }

Window::Window(int width, int height, bool fullscreen)
    : sdlWindow(nullptr, destroySDLWindow) {
    create(width, height, fullscreen);
}

void Window::create(int w, int h, bool fullscreen) {
    sdlWindow = SDLWindowPtr(createSDLWindow(w, h, fullscreen), destroySDLWindow);

    if (sdlWindow.get() == nullptr) {
        throw std::runtime_error(
            utils::stringFormat("Could not create SDLWindow: %s\n", SDL_GetError())
        );
    }

    width = w;
    height = h;
    logger::info("Window created\n");
}

void Window::setSize(int width, int height) {

}

void Window::setMode(WindowModes mode) {
    if (SDL_SetWindowFullscreen(sdlWindow.get(), mode) < 0) {
        logger::warn("Warning: Could not set fullscreen mode: %s\n", SDL_GetError());
    }
}

namespace {
    SDL_Window *createSDLWindow(int width, int height, bool fullscreen) {
        uint32_t flags = SDL_WINDOW_RESIZABLE;

        flags |= fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
        return SDL_CreateWindow(
                "raycaster",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                width,
                height,
                flags
        );
    }

    void destroySDLWindow(SDL_Window *win) {
        SDL_DestroyWindow(win);
        logger::info("Window destroyed\n");
    }
}
