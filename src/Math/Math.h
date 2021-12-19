#pragma once

#include <math.h>
#include <type_traits>

namespace Math
{
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

    template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    static T Clamp01(const T& value)
    {
        return Clamp(value, static_cast<T>(0.0), static_cast<T>(1.0));
    }

    template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    static T Lerp(const T& a, const T& b, float t)
    {
        return a + ((b - a) * Clamp01(t));
    }
}
