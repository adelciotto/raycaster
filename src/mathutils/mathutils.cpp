#include "mathutils/mathutils.h"

namespace mathutils {
    const float _toRadians = M_PI / 180.0f;
    const float _toDegrees = 180.0f / M_PI;

    float toRadians(float degrees) {
        return degrees * _toRadians;
    }

    float toDegrees(float radians) {
        return radians * _toDegrees;
    }
}
