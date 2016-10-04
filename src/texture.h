#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

struct Texture {
    int width;
    int height;
    int bytesPerPixel;
    std::vector<uint32_t> pixels;

    Texture();
    Texture(int width, int height);
    Texture(const std::string& file);

    void fromFile(const std::string& file);
};

#endif // TEXTURE_H
