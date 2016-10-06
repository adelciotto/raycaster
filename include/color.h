#ifndef COLOR_H
#define COLOR_H

#include "common.h"

struct Color {
    float r, g, b, a;

    Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) { }
    Color(float r, float g, float b) : r(r), g(g), b(b), a(1.0f) { }
    Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) { }
    Color(uint32_t hex) { set(hex); }

    inline Color& operator*= (float s) {
        r *= s;
        g *= s;
        b *= s;

        return *this;
    }

    inline Color operator/ (float s) {
        return Color(r / s, g / s, b / s, a);
    }

    inline Color& operator/= (float s) {
        r /= s;
        g /= s;
        b /= s;

        return *this;
    }

    inline void set(uint32_t hex) {
        r = (hex >> 24 & 0xff) / 255.0f;
        g = (hex >> 16 & 0xff) / 255.0f;
        b = (hex >> 8 & 0xff) / 255.0f;;
        a = (hex & 0xff) / 255.0f;;
    }

    inline uint32_t toHex() const {
        return Color::toHex(
            uint8_t(r*255),
            uint8_t(g*255),
            uint8_t(b*255),
            uint8_t(a*255)
        );
    }

    static uint32_t toHex(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return (r<<24) + (g<<16) + (b<<8) + a;
    }
};

#endif // COLOR_H
