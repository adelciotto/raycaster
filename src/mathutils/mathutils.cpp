#include "mathutils/mathutils.h"

namespace {
    const double _toRadians = M_PI / 180.0;
    const double _toDegrees = 180.0 / M_PI;
}

namespace mathutils {
    double toRadians(double degrees) {
        return degrees * _toRadians;
    }

    double toDegrees(double radians) {
        return radians * _toDegrees;
    }
}
