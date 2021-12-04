#include "Color.h"

Color::Color() : Color(0.0f, 0.0f, 0.0f, 0.0f) { }
Color::Color(float r, float g, float b) : Color(r, g, b, 1.0) { }
Color::Color(float r, float g, float b, float a)
{
    values[0] = r;
    values[1] = g;
    values[2] = b;
    values[3] = a;
}

Color::Color(const float* arr)
{
    for (int i = 0; i < 4; i++)
        values[i] = arr[i];
}

Color Color::operator +(const Color& other) const
{
    return Color(values[0] + other.values[0], values[1] + other.values[1], values[2] + other.values[2], values[3] + other.values[3]);
}

Color& Color::operator +=(const Color& other)
{
    *this = this->operator+(other);
    return *this;
}

Color Color::operator -(const Color& other) const
{
    return Color(values[0] - other.values[0], values[1] - other.values[1], values[2] + other.values[2], values[3] + other.values[3]);
}

Color& Color::operator -=(const Color& other)
{
    *this = this->operator-(other);
    return *this;
}

Color Color::operator *(float scalar) const
{
    return Color(values[0] * scalar, values[1] * scalar, values[2] * scalar, values[3] * scalar);
}

Color& Color::operator *=(float scalar)
{
    *this = this->operator*(scalar);
    return *this;
}

Color Color::operator /(float divisor) const
{
    float scalar = 1.0f / divisor;
    return Color(values[0] * scalar, values[1] * scalar, values[2] * scalar, values[3] * scalar);
}

Color& Color::operator /=(float divisor)
{
    *this = this->operator/(divisor);
    return *this;
}

Color Color::operator *(const Color& other) const
{
    return Color(values[0] * other.values[0], values[1] * other.values[1], values[2] * other.values[2], values[3] * other.values[3]);
}

Color& Color::operator *=(const Color& other)
{
    *this = this->operator*(other);
    return *this;
}

Color Color::operator /(const Color& other) const
{
    return Color(values[0] / other.values[0], values[1] / other.values[1], values[2] / other.values[2], values[3] / other.values[3]);
}

Color& Color::operator /=(const Color& other)
{
    *this = this->operator/(other);
    return *this;
}

Color Color::operator -() const
{
    return Color(-values[0], -values[1], -values[2], -values[3]);
}

Color::operator const float*() const
{
    return values;
}

float& Color::operator [](int index)
{
    return values[index];
}
