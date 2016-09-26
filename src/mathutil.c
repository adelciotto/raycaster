#include "mathutil.h"

static const float toRadians = M_PI / 180;
static const float toDegrees = 180 / M_PI;

void Vec2_Set(Vec2 *v, float x, float y) {
    v->x = x;
    v->y = y;
}

float MathUtil_ToRadians(float degrees) {
    return degrees * toRadians;
}

float MathUtil_ToDegrees(float radians) {
    return radians * toDegrees;
}
