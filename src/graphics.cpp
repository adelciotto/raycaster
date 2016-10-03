#include "graphics.h"
#include "window.h"
#include "color.h"

const int defaultBufferWidth = 640;
const int defaultBufferHeight = 360;
const int bytesPerPixel = 4;

static SDL_Renderer *createSDLRenderer(const Window& win);
static SDL_Texture *createSDLTexture(SDL_Renderer *rend, int width, int height);
static SDL_Surface *createSDLSurface(int width, int height);
static void destroySDLRenderer(SDL_Renderer *rend);
static void destroySDLTexture(SDL_Texture *tex);
static void destroySDLSurface(SDL_Surface *surface);

Graphics::Graphics(const Window& win)  
    : bufferWidth(defaultBufferWidth),
      bufferHeight(defaultBufferHeight),
      sdlRenderer(nullptr, SDL_DestroyRenderer),
      sdlTexture(nullptr, SDL_DestroyTexture),
      screen(nullptr, SDL_FreeSurface) {
    auto handleError = [](const std::string& resource) -> void {
        throw std::runtime_error(
            utils::stringFormat("Could not create %s: %s\n", resource.c_str(), SDL_GetError())
        );
    };

    sdlRenderer = SDLRendererPtr(createSDLRenderer(win), destroySDLRenderer);

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

    //int x2 = x + width;
    //int y2 = y + height;
    //if (width > height) {
        //for (int i = y; i <= y2; i++) {
            //drawHorizLine(i, x, x2, color); 
        //}
    //} else {
        //for (int i = x; i <= x2; i++) {
            //drawVertLine(i, y, y2, color);
        //}
    //}
}

void Graphics::setPixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= bufferWidth || y < 0 || y >= bufferHeight) return;

    uint32_t *bufp;
    auto screenPtr = screen.get();

    bufp = (uint32_t *)screenPtr->pixels + y * screenPtr->pitch / bytesPerPixel + x;
    *bufp = color;
}

static SDL_Renderer *createSDLRenderer(const Window& win) {
    int flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

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
