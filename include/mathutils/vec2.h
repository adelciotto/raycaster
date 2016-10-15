#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <ostream>

namespace mathutils {
    struct Vec2 {
        double x;
        double y;

        Vec2() : x(0), y(0) { }
        Vec2(double x, double y) : x(x), y(y) { }
        Vec2(const Vec2& v) : x(v.x), y(v.y) { }

        inline const Vec2 operator+ (double s) const {
            return Vec2(x + s, y + s);
        }

        inline const Vec2 operator+ (const Vec2& v) const {
            return Vec2(x + v.x, y + v.y);
        }

        inline const Vec2 operator- (double s) const {
            return Vec2(x - s, y - s);
        }

        inline const Vec2 operator- (const Vec2& v) const {
            return Vec2(x - v.x, y - v.y);
        }

        inline const Vec2 operator* (double s) const {
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

        void set(double s);
        void set(double x, double y);
        void set(const Vec2& v);
        double length();
        void rotate(double radians);
        std::string toString();

        friend std::ostream& operator<< (std::ostream& out, const Vec2& v);
    };

    inline std::ostream& operator<< (std::ostream& out, const Vec2& v) {
        return out << "[x: " << v.x << ", y: " << v.y << "]";
    }
}

#endif // VEC2_H
