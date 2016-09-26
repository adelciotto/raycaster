#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <cmath>
#include <ostream>

namespace mathutils {
    const float _toRadians = M_PI / 180.0f;
    const float _toDegrees = 180.0f / M_PI;

    inline float toRadians(float degrees) {
        return degrees * _toRadians;
    }

    inline float toDegrees(float radians) {
        return radians * _toDegrees;
    }

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

        void set(float s) { 
            this->x = s;
            this->y = s;
        }
        
        void set(float x, float y) { 
            this->x = x;
            this->y = y;
        }

        void set(const Vec2& v) { 
            this->x = v.x;
            this->y = v.y;
        }

        float length() {
            return (float)sqrt(x*x + y*y);
        }

        void rotate(float radians) {
            float cs = (float)cos(radians);
            float sn = (float)sin(radians);

            float px = x * cs - y * sn;
            float py = x * sn + y * cs;

            x = px;
            y = py;
        }

        friend std::ostream& operator<< (std::ostream& out, const Vec2& v);
    };

    inline std::ostream& operator<< (std::ostream& out, const Vec2& v) {
        return out << "[x: " << v.x << ", y: " << v.y << "]";
    }
};

#endif // MATHUTIL_H
