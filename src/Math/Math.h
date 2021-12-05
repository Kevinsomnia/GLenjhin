#pragma once

#include <math.h>
#include <type_traits>

#define PI 3.14159265358979323846
#define PI_FLOAT 3.14159265358979323846f

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
static T Min(const T& a, const T& b)
{
    return (a < b) ? a : b;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
static T Max(const T& a, const T& b)
{
    return (a > b) ? a : b;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
static T Clamp(const T& value, const T& min, const T& max)
{
    if (value < min)
    {
        return min;
    }
    else if (value > max)
    {
        return max;
    }

    return value;
}

static float WrapAngle(float theta)
{
    const float TWO_PI = PI_FLOAT * 2.0f;
    return theta - TWO_PI * floor(theta / TWO_PI);
}
