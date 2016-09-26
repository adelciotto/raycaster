#include "graphics.h"
#include "window.h"

const int bufferWidth = 320;
const int bufferHeight = 240;
const int bytesPerPixel = 4;

static void setPixel(Graphics *g, int x, int y, uint32_t color);

void Graphics_Init(Graphics *g, Window *w) {
    int flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *rend = SDL_CreateRenderer(w->sdlWindow, -1, flags);

    if (rend == NULL) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_RenderSetLogicalSize(rend, bufferWidth, bufferHeight);
    SDL_Texture *tex = SDL_CreateTexture(
            rend,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING, 
            bufferWidth,
            bufferHeight
    );

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

    SDL_Surface *sur = SDL_CreateRGBSurface(
            0,
            bufferWidth,
            bufferHeight,
            32,
            rmask,
            gmask,
            bmask,
            amask);

    if (sur == NULL) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    g->bufferWidth = bufferWidth;
    g->bufferHeight = bufferHeight;
    g->screen = sur;
    g->sdlTexture = tex;
    g->sdlRenderer = rend;
}

void Graphics_Destroy(Graphics *g) {
    SDL_FreeSurface(g->screen);
    SDL_DestroyTexture(g->sdlTexture);
    SDL_DestroyRenderer(g->sdlRenderer);
}

void Graphics_Clear(Graphics *g, uint32_t color) {
    SDL_FillRect(g->screen, NULL, color);
}

void Graphics_Present(Graphics *g) {
    SDL_UpdateTexture(g->sdlTexture, NULL, g->screen->pixels, g->screen->pitch);

    SDL_RenderClear(g->sdlRenderer);
    SDL_RenderCopy(g->sdlRenderer, g->sdlTexture, NULL, NULL);
    SDL_RenderPresent(g->sdlRenderer);
}


void Graphics_DrawVertLine(Graphics *g, int x, int y1, int y2, uint32_t color) {
    if (y1 == y2) return;

    // If drawing upwards, swap y1 and y2 and draw downwards.
    if (y2 < y1) {
        y1 += y2;
        y2 = y1 - y2;
        y1 -= y2;
    }

    if (y1 < 0) y1 = 0;
    if (y2 > g->bufferHeight) y2 = g->bufferHeight;

    for (int y = y1; y <= y2; y++) {
        setPixel(g, x, y, color);
    }
}

void Graphics_DrawLine(Graphics *g, int x1, int y1, int x2, int y2, uint32_t color) {
    // Using bresenhams line drawing algorithm.
    int dx = abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = abs(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;
    int e2;

    for (;;) {
        setPixel(g, x1, y1, color);

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

static void setPixel(Graphics *g, int x, int y, uint32_t color) {
    if (x < 0 || x >= bufferWidth || y < 0 || y >= g->bufferHeight) return;

    uint32_t *bufp;
    
    bufp = (uint32_t *)g->screen->pixels + y * g->screen->pitch / bytesPerPixel + x;
    *bufp = color;
}

