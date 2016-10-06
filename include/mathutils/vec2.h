#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <ostream>

namespace mathutils {
    struct Vec2 {
        float x;
        float y;

        Vec2() : x(0), y(0) { }
        Vec2(float x, float y) : x(x), y(y) { }
        Vec2(const Vec2& v) : x(v.x), y(v.y) { }

        inline const Vec2 operator+ (float s) const {
            return Vec2(x + s, y + s);
        }

        inline const Vec2 operator+ (const Vec2& v) const {
            return Vec2(x + v.x, y + v.y);
        }

        inline const Vec2 operator- (float s) const {
            return Vec2(x - s, y - s);
        }

        inline const Vec2 operator- (const Vec2& v) const {
            return Vec2(x - v.x, y - v.y);
        }

        inline const Vec2 operator* (float s) const {
            return Vec2(x * s, y * s);
        }

        inline const Vec2 operator* (const Vec2& v) const {
            return Vec2(x * v.x, y * v.y);
        }

        inline Vec2& operator+= (const Vec2& v) {
            x += v.x;
            y += v.y;

            return *this;
        }

        void set(float s);
        void set(float x, float y);
        void set(const Vec2& v);
        float length();
        void rotate(float radians);

        friend std::ostream& operator<< (std::ostream& out, const Vec2& v);
    };

    inline std::ostream& operator<< (std::ostream& out, const Vec2& v) {
        return out << "[x: " << v.x << ", y: " << v.y << "]";
    }
}

#endif // VEC2_H
