#include "graphics.h"
#include "window.h"
#include "color.h"

#include "stb_image.h"

static const int defaultBufferWidth = 640;
static const int defaultBufferHeight = 360;
static const int bytesPerPixel = 4;

static SDL_Renderer *createSDLRenderer(const Window& win, bool vsync);
static SDL_Texture *createSDLTexture(SDL_Renderer *rend, int width, int height);
static SDL_Surface *createSDLSurface(int width, int height);
static void destroySDLRenderer(SDL_Renderer *rend);
static void destroySDLTexture(SDL_Texture *tex);
static void destroySDLSurface(SDL_Surface *surface);

Graphics::Graphics()
    : bufferWidth(defaultBufferWidth),
      bufferHeight(defaultBufferHeight),
      sdlRenderer(nullptr, SDL_DestroyRenderer),
      sdlTexture(nullptr, SDL_DestroyTexture),
      screen(nullptr, SDL_FreeSurface) { }

Graphics::Graphics(const Window& win, bool vsync)
    : bufferWidth(defaultBufferWidth),
      bufferHeight(defaultBufferHeight),
      sdlRenderer(nullptr, SDL_DestroyRenderer),
      sdlTexture(nullptr, SDL_DestroyTexture),
      screen(nullptr, SDL_FreeSurface) {
    create(win, vsync);
}

void Graphics::create(const Window& win, bool vsync) {
    auto handleError = [](const std::string& resource) -> void {
        throw std::runtime_error(
            utils::stringFormat("Could not create %s: %s\n", resource.c_str(), SDL_GetError())
        );
    };

    sdlRenderer = SDLRendererPtr(createSDLRenderer(win, vsync), destroySDLRenderer);

    if (sdlRenderer.get() == nullptr) handleError("SDLRenderer");
    logger::info("SDL Renderer created\n");

    sdlTexture = SDLTexturePtr(createSDLTexture(
        sdlRenderer.get(),
        bufferWidth,
        bufferHeight),
        destroySDLTexture
    );

    if (sdlTexture.get() == nullptr) handleError("SDLTexture");
    logger::info("SDL Texture created\n");

    screen = SDLSurfacePtr(createSDLSurface(
        bufferWidth,
        bufferHeight),
        destroySDLSurface
    );

    if (screen.get() == nullptr) handleError("SDLSurface");
    logger::info("SDL Surface created\n");

    generateBitmapFont();
}

void Graphics::clear(uint32_t color) {
    SDL_FillRect(screen.get(), NULL, color);
}

void Graphics::present() {
    auto renderer = sdlRenderer.get();
    auto texture = sdlTexture.get();
    auto screenPtr = screen.get();

    SDL_UpdateTexture(texture, NULL, screenPtr->pixels, screenPtr->pitch);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void Graphics::drawVertLine(int x, int y1, int y2, uint32_t color) {
    if (y1 == y2) return;

    // If drawing upwards, swap y1 and y2 and draw downwards.
    if (y2 < y1) {
        y1 += y2;
        y2 = y1 - y2;
        y1 -= y2;
    }

    if (y1 < 0) y1 = 0;
    if (y2 > bufferHeight) y2 = bufferHeight;

    for (int y = y1; y <= y2; y++) {
        setPixel(x, y, color);
    }
}

void Graphics::drawHorizLine(int y, int x1, int x2, uint32_t color) {
    if (x1 == x2) return;

    // If drawing lefwards, swap x1 and x2 and draw rightwards.
    if (x2 < x1) {
        x1 += x2;
        x2 = x1 - x2;
        x1 -= x2;
    }

    if (x1 < 0) x1 = 0;
    if (x2 > bufferWidth) x2 = bufferWidth;

    for (int x = x1; x <= x2; x++) {
        setPixel(x, y, color);
    }
}

void Graphics::drawLine(int x1, int y1, int x2, int y2, uint32_t color) {
    int dx = abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = abs(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;
    int e2;

    for (;;) {
        setPixel(x1, y1, color);

        if (x1 == x2 && y1 == y2) break;

        e2 = err;

        if (e2 > -dx) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y1 += sy;
        }
    }
}

void Graphics::drawRect(int x, int y, int width, int height, uint32_t color) {
    const SDL_Rect rect{x, y, width, height};

    SDL_FillRect(screen.get(), &rect, color);
}

int Graphics::drawString(const std::string& text, int x, int y, uint32_t color) {
    return drawString(text, x, y, color, false, 0x00000000);
}

int Graphics::drawString(const std::string& text, int x, int y, uint32_t color, bool useBg,
                         uint32_t bgColor) {
    for (size_t i = 0; i < text.size(); i++) {
        drawLetter(text[i], x, y, color, useBg, bgColor);
        x += CharSize;

        if (x > bufferWidth) y += LineHeight;
    }

    return y += LineHeight;
}

void Graphics::setPixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= bufferWidth || y < 0 || y >= bufferHeight) return;

    uint32_t *bufp;
    auto screenPtr = screen.get();

    bufp = (uint32_t *)screenPtr->pixels + y * screenPtr->pitch / bytesPerPixel + x;
    *bufp = color;
}

void Graphics::generateBitmapFont() {
    int w, h, bpp;
    uint8_t *data = stbi_load("assets/textures/bitmapfont.png", &w, &h, &bpp, 0);
    int numCells = w / CharSize;

    for (int c = 0; c < NumChars; c++) {
        for (int y = 0; y < CharSize; y++) {
            for (int x = 0; x < CharSize; x++) {
                int i = bpp * w * (CharSize * (c / numCells) + y) + bpp * (CharSize * (c % numCells) + x);
                bitmapFont[c][x][y] = data[i] == 0xFF;
            }
        }
    }

    stbi_image_free(data);
}

void Graphics::drawLetter(unsigned char n, int x, int y, uint32_t color, bool useBg, uint32_t bgColor) {
    for (int v = 0; v < CharSize; v++) {
        for (int u = 0; u < CharSize; u++) {
            if (bitmapFont[n][u][v]) {
                setPixel(x + u, y + v, color);
            } else if (useBg) {
                setPixel(x + u, y + v, bgColor);
            }
        }
    }
}

static SDL_Renderer *createSDLRenderer(const Window& win, bool vsync) {
    int flags = SDL_RENDERER_ACCELERATED;

    flags = vsync ? flags | SDL_RENDERER_PRESENTVSYNC : flags;
    return SDL_CreateRenderer(win.getSDLWindow(), -1, flags);
}

static SDL_Texture *createSDLTexture(SDL_Renderer *rend, int width, int height) {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_RenderSetLogicalSize(rend, width, height);
    
    return SDL_CreateTexture(
        rend,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING, 
        width,
        height
    );
}

static SDL_Surface *createSDLSurface(int width, int height) {
    uint32_t rmask, gmask, bmask, amask;

// SDL interprets each pixel as a 32-bit number, so our masks must depend
// on the endianness (byte order) of the machine.
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    return SDL_CreateRGBSurface(
        0,
        width,
        height,
        32,
        rmask,
        gmask,
        bmask,
        amask
    );
}

static void destroySDLRenderer(SDL_Renderer *rend) {
    SDL_DestroyRenderer(rend);
    logger::info("SDL Renderer destroyed\n");
}

static void destroySDLTexture(SDL_Texture *tex) {
    SDL_DestroyTexture(tex);
    logger::info("SDL Texture destroyed\n");
}

static void destroySDLSurface(SDL_Surface *surface) {
    SDL_FreeSurface(surface);
    logger::info("SDL Surface destroyed\n");
}
