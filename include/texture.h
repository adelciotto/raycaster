#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"
#include "iresource.h"

class Texture : public IResource {
public:
    Texture();
    Texture(const std::string& filename);

    int getWidth() const;
    int getHeight() const;
    int getBytesPerPixel() const;
    uint32_t getPixel(int x, int y) const;
    void setPixel(int x, int y, uint32_t color);
    
    void load();

private:
    int width;
    int height;
    int bytesPerPixel;
    std::vector<uint32_t> pixels;
};

#endif // TEXTURE_H
