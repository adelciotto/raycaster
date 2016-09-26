#ifndef MATHUTIL_H

#include <math.h>

typedef struct {
    float x;
    float y;
} Vec2;

void Vec2_Set(Vec2 *v, float x, float y);

float MathUtil_ToRadians(float degrees);
float MathUtil_ToDegrees(float radians);

#endif // MATHUTIL_H
