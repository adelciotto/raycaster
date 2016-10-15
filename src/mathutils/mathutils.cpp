#include "mathutils/mathutils.h"

namespace mathutils {
    const double _toRadians = M_PI / 180.0;
    const double _toDegrees = 180.0 / M_PI;

    double toRadians(double degrees) {
        return degrees * _toRadians;
    }

    double toDegrees(double radians) {
        return radians * _toDegrees;
    }
}
