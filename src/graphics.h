#ifndef GRAPHICS_H

#include "common.h"

struct window_t;

typedef struct {
    int bufferWidth;
    int bufferHeight;

    SDL_Surface *screen;
    SDL_Texture *sdlTexture;
    SDL_Renderer *sdlRenderer;
} Graphics;

void Graphics_Init(Graphics *g, struct window_t *w);
void Graphics_Destroy(Graphics *g);
void Graphics_Clear(Graphics *g, uint32_t color);
void Graphics_Present(Graphics *g);
void Graphics_DrawVertLine(Graphics *g, int x, int y1, int y2, uint32_t color);
void Graphics_DrawLine(Graphics *g, int x1, int y1, int x2, int y2, uint32_t color);

#endif // GRAPHICS_H
