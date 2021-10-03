#ifndef CONVERT_H
#define CONVERT_H

#include <time.h>
#include "Math.h"

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

#endif // CONVERT_H
