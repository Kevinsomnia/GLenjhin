#ifndef MATH_H
#define MATH_H

#include <math.h>

static const double PI = 3.14159265358979323846;
static const float PI_FLOAT = 3.14159265358979323846f;

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

#endif // MATH_H
