#include "mathutils/vec2.h"
#include "utils.h"

namespace mathutils {
    Vec2::Vec2() : 
        x(0), 
        y(0) { }

    Vec2::Vec2(double x, double y) :
        x(x),
        y(y) { }

    const Vec2 Vec2::operator+ (double s) const {
        return Vec2(x + s, y + s);
    }

    const Vec2 Vec2::operator+ (const Vec2& v) const {
        return Vec2(x + v.x, y + v.y);
    }

    const Vec2 Vec2::operator- (double s) const {
        return Vec2(x - s, y - s);
    }

    const Vec2 Vec2::operator- (const Vec2& v) const {
        return Vec2(x - v.x, y - v.y);
    }

    const Vec2 Vec2::operator* (double s) const {
        return Vec2(x * s, y * s);
    }

    const Vec2 Vec2::operator* (const Vec2& v) const {
        return Vec2(x * v.x, y * v.y);
    }

    Vec2& Vec2::operator+= (const Vec2& v) {
        x += v.x;
        y += v.y;

        return *this;
    }

    void Vec2::set(double s) {
        this->x = s;
        this->y = s;
    }

    void Vec2::set(double x, double y) {
        this->x = x;
        this->y = y;
    }

    void Vec2::set(const Vec2& v) { 
        this->x = v.x;
        this->y = v.y;
    }

    double Vec2::length() {
        return std::sqrt(x*x + y*y);
    }

    void Vec2::rotate(double radians) {
        double cs = (double)cos(radians);
        double sn = (double)sin(radians);

        double px = x * cs - y * sn;
        double py = x * sn + y * cs;

        x = px;
        y = py;
    }

    std::string Vec2::toString() const {
        return utils::stringFormat("[x: %.2f, y: %.2f]", x, y);
    }
}
