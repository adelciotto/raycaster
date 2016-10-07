#include "mathutils/vec2.h"
#include "utils.h"

namespace mathutils {
    void Vec2::set(float s) { 
        this->x = s;
        this->y = s;
    }

    void Vec2::set(float x, float y) { 
        this->x = x;
        this->y = y;
    }

    void Vec2::set(const Vec2& v) { 
        this->x = v.x;
        this->y = v.y;
    }

    float Vec2::length() {
        return (float)sqrt(x*x + y*y);
    }

    void Vec2::rotate(float radians) {
        float cs = (float)cos(radians);
        float sn = (float)sin(radians);

        float px = x * cs - y * sn;
        float py = x * sn + y * cs;

        x = px;
        y = py;
    }

    std::string Vec2::toString() {
        return utils::stringFormat("[x: %.2f, y: %.2f]", x, y);
    }
}
