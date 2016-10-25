#include "texture.h"
#include "color.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace {
    const int defaultWidth = 64;
    const int defaultHeight = 64;
    const int defaultBpp = 4;
}

Texture::Texture()
        : width(defaultWidth),
          height(defaultHeight),
          bytesPerPixel(defaultBpp) { }

Texture::Texture(const std::string& filename)
        : IResource(filename) { }

int Texture::getWidth() const {
    return width;
}

int Texture::getHeight() const {
    return height;
}

int Texture::getBytesPerPixel() const {
    return bytesPerPixel;
}

uint32_t Texture::getPixel(int x, int y) const {
    return pixels[height * x + y];
}

void Texture::setPixel(int x, int y, uint32_t color) { }

void Texture::load() {
    int w, h, bbp;
    uint8_t *data = stbi_load(filename.c_str(), &w, &h, &bbp, 0);

    if (data == nullptr) {
        throw ResourceException("Image could not be loaded", filename);
    }

    int length = w * h * bbp;

    pixels.reserve(w*h);
    for (int i = 0; i < length; i += bbp) {
        uint32_t color = Color::toHex(data[i], data[i+1], data[i+2], 0xFF);

        pixels.push_back(color);
    }

    stbi_image_free(data);

    // Swap texture x/y because they will be used as vertical stripes.
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < x; y++) {
            std::swap(pixels[w * y + x], pixels[w * x + y]);
        }
    }

    width = w;
    height = h;
    bytesPerPixel = bbp;
}
