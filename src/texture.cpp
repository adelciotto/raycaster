#include "texture.h"
#include "color.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static const int defaultWidth = 64;
static const int defaultHeight = 64;
static const int defaultBpp = 4;

Texture::Texture() 
    : width(defaultWidth),
      height(defaultHeight),
      bytesPerPixel(defaultBpp) { }

Texture::Texture(int width, int height) {

}

Texture::Texture(const std::string& file) {
    fromFile(file);
}

void Texture::fromFile(const std::string& file) {
    int w, h, bbp;
    uint8_t *data = stbi_load(file.c_str(), &w, &h, &bbp, 0);

    if (data == nullptr) {
        logger::warn("Image %s could not be loaded\n", file.c_str());
        return;
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
