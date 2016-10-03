#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "common.h"

class Window;

typedef std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer *)> SDLRendererPtr;
typedef std::unique_ptr<SDL_Texture, void(*)(SDL_Texture *)> SDLTexturePtr;
typedef std::unique_ptr<SDL_Surface, void(*)(SDL_Surface *)> SDLSurfacePtr;

class Graphics {
    int bufferWidth;
    int bufferHeight;
    SDLRendererPtr sdlRenderer;
    SDLTexturePtr sdlTexture;
    SDLSurfacePtr screen;

public:
    Graphics(const Window& win, bool vsync);

    int width() const { return bufferWidth; }
    int height() const { return bufferHeight; }
    void clear(uint32_t color);
    void present();
    void drawVertLine(int x, int y1, int y2, uint32_t color);
    void drawHorizLine(int y, int x1, int x2, uint32_t color);
    void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
    void drawRect(int x, int y, int width, int height, uint32_t color);
    void setPixel(int x, int y, uint32_t color);
};

#endif // GRAPHICS_H
