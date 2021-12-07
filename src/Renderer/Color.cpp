#include "Color.h"

// ==================================================
// Color (floating point)
// ==================================================

Color::Color() : Color(0.0f, 0.0f, 0.0f, 0.0f) { }
Color::Color(float r, float g, float b) : Color(r, g, b, 1.0) { }
Color::Color(float r, float g, float b, float a)
{
    values[0] = r;
    values[1] = g;
    values[2] = b;
    values[3] = a;
}

Color::Color(const float* arr, bool alpha)
{
    memcpy(values, arr, (alpha ? 4 : 3) * sizeof(float));
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
    return Color(values[0] - other.values[0], values[1] - other.values[1], values[2] - other.values[2], values[3] - other.values[3]);
}

Color& Color::operator -=(const Color& other)
{
    *this = this->operator-(other);
    return *this;
}

Color Color::operator *(float scalar) const
{
    return Color(values[0] * scalar, values[1] * scalar, values[2] * scalar, values[3]);
}

Color& Color::operator *=(float scalar)
{
    *this = this->operator*(scalar);
    return *this;
}

Color Color::operator /(float divisor) const
{
    float scalar = 1.0f / divisor;
    return Color(values[0] * scalar, values[1] * scalar, values[2] * scalar, values[3]);
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

Color::operator const float*() const
{
    return values;
}

Color::operator ColorByte() const
{
    return ColorByte(
        static_cast<uint8_t>(values[0] * 255.0f),
        static_cast<uint8_t>(values[1] * 255.0f),
        static_cast<uint8_t>(values[2] * 255.0f),
        static_cast<uint8_t>(values[3] * 255.0f)
    );
}

float& Color::operator [](int index)
{
    return values[index];
}



// ==================================================
// Color (byte/integer)
// ==================================================

ColorByte::ColorByte() : ColorByte(0, 0, 0, 0) { }
ColorByte::ColorByte(uint8_t r, uint8_t g, uint8_t b) : ColorByte(r, g, b, 255) { }
ColorByte::ColorByte(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    values[0] = r;
    values[1] = g;
    values[2] = b;
    values[3] = a;
}

ColorByte::ColorByte(const uint8_t* arr, bool alpha)
{
    memcpy(values, arr, (alpha ? 4 : 3) * sizeof(uint8_t));
}

ColorByte ColorByte::operator +(const ColorByte& other) const
{
    return ColorByte(
        values[0] < 255 - other.values[0] ? values[0] + other.values[0] : 255,
        values[1] < 255 - other.values[1] ? values[1] + other.values[1] : 255,
        values[2] < 255 - other.values[2] ? values[2] + other.values[2] : 255,
        values[3] < 255 - other.values[3] ? values[3] + other.values[3] : 255
    );
}

ColorByte& ColorByte::operator +=(const ColorByte& other)
{
    *this = this->operator+(other);
    return *this;
}

ColorByte ColorByte::operator -(const ColorByte& other) const
{
    return ColorByte(
        other.values[0] < values[0] ? values[0] - other.values[0] : 0,
        other.values[1] < values[1] ? values[1] - other.values[1] : 0,
        other.values[2] < values[2] ? values[2] - other.values[2] : 0,
        other.values[3] < values[3] ? values[3] - other.values[3] : 0
    );
}

ColorByte& ColorByte::operator -=(const ColorByte& other)
{
    *this = this->operator-(other);
    return *this;
}

ColorByte ColorByte::operator *(float scalar) const
{
    float clampedScalar = Clamp(scalar, 0.0f, 1.0f);
    return ColorByte(
        static_cast<uint8_t>(values[0] * clampedScalar),
        static_cast<uint8_t>(values[1] * clampedScalar),
        static_cast<uint8_t>(values[2] * clampedScalar),
        values[3]
    );
}

ColorByte& ColorByte::operator *=(float scalar)
{
    *this = this->operator*(scalar);
    return *this;
}

ColorByte ColorByte::operator /(float divisor) const
{
    float clampedScalar = Clamp(1.0f / divisor, 0.0f, 1.0f);
    return ColorByte(
        static_cast<uint8_t>(values[0] * clampedScalar),
        static_cast<uint8_t>(values[1] * clampedScalar),
        static_cast<uint8_t>(values[2] * clampedScalar),
        values[3]
    );
}

ColorByte& ColorByte::operator /=(float divisor)
{
    *this = this->operator/(divisor);
    return *this;
}

ColorByte ColorByte::operator *(const ColorByte& other) const
{
    const float FACTOR = 1.0f / 255.0f;
    return ColorByte(
        static_cast<uint8_t>((values[0] * other.values[0]) * FACTOR),
        static_cast<uint8_t>((values[1] * other.values[1]) * FACTOR),
        static_cast<uint8_t>((values[2] * other.values[2]) * FACTOR),
        static_cast<uint8_t>((values[3] * other.values[3]) * FACTOR)
    );
}

ColorByte& ColorByte::operator *=(const ColorByte& other)
{
    *this = this->operator*(other);
    return *this;
}

ColorByte ColorByte::operator /(const ColorByte& other) const
{
    float r = other.values[0] > 0 ? values[0] / static_cast<float>(other.values[0]) : 0.0f;
    float g = other.values[1] > 0 ? values[1] / static_cast<float>(other.values[1]) : 0.0f;
    float b = other.values[2] > 0 ? values[2] / static_cast<float>(other.values[2]) : 0.0f;
    float a = other.values[3] > 0 ? values[3] / static_cast<float>(other.values[3]) : 0.0f;

    return ColorByte(
        static_cast<uint8_t>(Clamp(r, 0.0f, 1.0f) * 255.0f),
        static_cast<uint8_t>(Clamp(g, 0.0f, 1.0f) * 255.0f),
        static_cast<uint8_t>(Clamp(b, 0.0f, 1.0f) * 255.0f),
        static_cast<uint8_t>(Clamp(a, 0.0f, 1.0f) * 255.0f)
    );
}

ColorByte& ColorByte::operator /=(const ColorByte& other)
{
    *this = this->operator/(other);
    return *this;
}

ColorByte::operator const uint8_t*() const
{
    return values;
}

ColorByte::operator Color() const
{
    const float NORMALIZE_FACTOR = 1.0f / 255.0f;
    return Color(
        values[0] * NORMALIZE_FACTOR,
        values[1] * NORMALIZE_FACTOR,
        values[2] * NORMALIZE_FACTOR,
        values[3] * NORMALIZE_FACTOR
    );
}

uint8_t& ColorByte::operator [](int index)
{
    return values[index];
}
