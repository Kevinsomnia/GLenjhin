#pragma once

#include <time.h>
#include "Math.h"
#include "Vector.h"

#define DEG_TO_RAD PI_FLOAT / 180.0f
#define RAD_TO_DEG 180.0f / PI_FLOAT

inline float degToRad(float degrees)
{
    return degrees * DEG_TO_RAD;
}

inline float radToDeg(float radians)
{
    return radians * RAD_TO_DEG;
}

inline Vector3 rotationToRad(const Vector3& rotDeg)
{
    return Vector3(degToRad(rotDeg.getX()), degToRad(rotDeg.getY()), degToRad(rotDeg.getZ()));
}

inline Vector3 rotationToDeg(const Vector3& rotRad)
{
    return Vector3(radToDeg(rotRad.getX()), radToDeg(rotRad.getY()), radToDeg(rotRad.getZ()));
}
