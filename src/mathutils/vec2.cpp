#include "mathutils/vec2.h"
#include "utils.h"

namespace mathutils {
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
        return (double)sqrt(x*x + y*y);
    }

    void Vec2::rotate(double radians) {
        double cs = (double)cos(radians);
        double sn = (double)sin(radians);

        double px = x * cs - y * sn;
        double py = x * sn + y * cs;

        x = px;
        y = py;
    }

    std::string Vec2::toString() {
        return utils::stringFormat("[x: %.2f, y: %.2f]", x, y);
    }
}
