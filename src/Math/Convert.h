#ifndef CONVERT_H
#define CONVERT_H

#include <time.h>
#include "Math.h"
#include "Vector.h"

#define DEG_TO_RAD PI_FLOAT / 180.0f
#define RAD_TO_DEG 180.0f / PI_FLOAT

inline double clockTicksToSeconds(clock_t clk)
{
	return clk / (double)CLOCKS_PER_SEC;
}

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
	return Vector3(degToRad(rotDeg.x), degToRad(rotDeg.y), degToRad(rotDeg.z));
}

inline Vector3 rotationToDeg(const Vector3& rotRad)
{
	return Vector3(radToDeg(rotRad.x), radToDeg(rotRad.y), radToDeg(rotRad.z));
}

#endif // CONVERT_H
