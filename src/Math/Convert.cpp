#include "Convert.h"

double clockTicksToSeconds(clock_t clk)
{
	return clk / (double)CLOCKS_PER_SEC;
}

float degToRad(float degrees)
{
	return degrees * PI_FLOAT / 180.0f;
}

float radToDeg(float radians)
{
	return radians * 180.0f / PI_FLOAT;
}
