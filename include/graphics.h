#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "common.h"

class Window;

typedef std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer *)> SDLRendererPtr;
typedef std::unique_ptr<SDL_Texture, void(*)(SDL_Texture *)> SDLTexturePtr;
typedef std::unique_ptr<SDL_Surface, void(*)(SDL_Surface *)> SDLSurfacePtr;

class Graphics {
public:
    Graphics();
    Graphics(const Window& win, bool vsync);

    int width() const { return bufferWidth; }
    int height() const { return bufferHeight; }
    void create(const Window& win, bool vsync);
    void clear(uint32_t color);
    void present();
    void drawVertLine(int x, int y1, int y2, uint32_t color);
    void drawHorizLine(int y, int x1, int x2, uint32_t color);
    void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
    void drawRect(int x, int y, int width, int height, uint32_t color);
    int drawString(const std::string& text, int x, int y, uint32_t color);
    int drawString(const std::string& text, int x, int y, uint32_t color, bool useBg,
                   uint32_t bgColor);
    void setPixel(int x, int y, uint32_t color);

private:
    static const int NumChars = 256;
    static const int CharSize = 8;
    static const int LineHeight = CharSize * 1.5;

    int bufferWidth;
    int bufferHeight;
    SDLRendererPtr sdlRenderer;
    SDLTexturePtr sdlTexture;
    SDLSurfacePtr screen;
    bool bitmapFont[NumChars][CharSize][CharSize];

    void generateBitmapFont();
    void drawLetter(unsigned char n, int x, int y, uint32_t color, bool useBg, uint32_t bgColor);
};

#endif // GRAPHICS_H
