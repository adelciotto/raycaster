#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <ostream>

namespace mathutils {
    struct Vec2 {
        double x;
        double y;

        Vec2();
        Vec2(double x, double y);

        const Vec2 operator+ (double s) const;
        const Vec2 operator+ (const Vec2& v) const;
        const Vec2 operator- (double s) const;
        const Vec2 operator- (const Vec2& v) const;
        const Vec2 operator* (double s) const;
        const Vec2 operator* (const Vec2& v) const;

        Vec2& operator+= (const Vec2& v);

        void set(double s);
        void set(double x, double y);
        void set(const Vec2& v);
        double length();
        void rotate(double radians);
        std::string toString() const;

        friend std::ostream& operator<< (std::ostream& out, const Vec2& v);
    };

    inline std::ostream& operator<< (std::ostream& out, const Vec2& v) {
        return out << "[x: " << v.x << ", y: " << v.y << "]";
    }
}

#endif // VEC2_H
