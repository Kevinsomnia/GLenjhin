#include "Color.h"

// ==================================================
// Color (floating point)
// ==================================================

Color::Color() : Color(0.0f, 0.0f, 0.0f, 0.0f) { }
Color::Color(float r, float g, float b) : Color(r, g, b, 1.0) { }
Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) { }

Color::Color(const float* arr, bool alpha)
{
    memcpy(&r, arr, (alpha ? 4 : 3) * sizeof(float));
}

Color Color::operator +(const Color& other) const
{
    return Color(r + other.r, g + other.g, b + other.b, a + other.a);
}

Color& Color::operator +=(const Color& other)
{
    *this = this->operator+(other);
    return *this;
}

Color Color::operator -(const Color& other) const
{
    return Color(r - other.r, g - other.g, b - other.b, a - other.a);
}

Color& Color::operator -=(const Color& other)
{
    *this = this->operator-(other);
    return *this;
}

Color Color::operator *(float scalar) const
{
    return Color(r * scalar, g * scalar, b * scalar, a);
}

Color& Color::operator *=(float scalar)
{
    *this = this->operator*(scalar);
    return *this;
}

Color Color::operator /(float divisor) const
{
    float scalar = 1.0f / divisor;
    return Color(r * scalar, g * scalar, b * scalar, a);
}

Color& Color::operator /=(float divisor)
{
    *this = this->operator/(divisor);
    return *this;
}

Color Color::operator *(const Color& other) const
{
    return Color(r * other.r, g * other.g, b * other.b, a * other.a);
}

Color& Color::operator *=(const Color& other)
{
    *this = this->operator*(other);
    return *this;
}

Color Color::operator /(const Color& other) const
{
    return Color(r / other.r, g / other.g, b / other.b, a / other.a);
}

Color& Color::operator /=(const Color& other)
{
    *this = this->operator/(other);
    return *this;
}

Color::operator const float*() const
{
    return &r;
}

Color::operator ColorByte() const
{
    return ColorByte(
        static_cast<uint8_t>(r * 255.0f),
        static_cast<uint8_t>(g * 255.0f),
        static_cast<uint8_t>(b * 255.0f),
        static_cast<uint8_t>(a * 255.0f)
    );
}

float& Color::operator [](int index)
{
    switch (index)
    {
        case 0:
            return r;
        case 1:
            return g;
        case 2:
            return b;
        case 3:
            return a;
        default:
            throw "Index out of range";
    }
}



// ==================================================
// Color (byte/integer)
// ==================================================

ColorByte::ColorByte() : ColorByte(0, 0, 0, 0) { }
ColorByte::ColorByte(uint8_t r, uint8_t g, uint8_t b) : ColorByte(r, g, b, 255) { }
ColorByte::ColorByte(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) { }

ColorByte::ColorByte(const uint8_t* arr, bool alpha)
{
    memcpy(&r, arr, (alpha ? 4 : 3) * sizeof(uint8_t));
}

ColorByte ColorByte::operator +(const ColorByte& other) const
{
    return ColorByte(
        r < 255 - other.r ? r + other.r : 255,
        g < 255 - other.g ? g + other.g : 255,
        b < 255 - other.b ? b + other.b : 255,
        a < 255 - other.a ? a + other.a : 255
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
        other.r < r ? r - other.r : 0,
        other.g < g ? g - other.g : 0,
        other.b < b ? b - other.b : 0,
        other.a < a ? a - other.a : 0
    );
}

ColorByte& ColorByte::operator -=(const ColorByte& other)
{
    *this = this->operator-(other);
    return *this;
}

ColorByte ColorByte::operator *(float scalar) const
{
    return ColorByte(
        static_cast<uint8_t>(Math::Clamp(r * scalar, 0.0f, 255.0f)),
        static_cast<uint8_t>(Math::Clamp(g * scalar, 0.0f, 255.0f)),
        static_cast<uint8_t>(Math::Clamp(b * scalar, 0.0f, 255.0f)),
        a
    );
}

ColorByte& ColorByte::operator *=(float scalar)
{
    *this = this->operator*(scalar);
    return *this;
}

ColorByte ColorByte::operator /(float divisor) const
{
    float scalar = 1.0f / divisor;
    return ColorByte(
        static_cast<uint8_t>(Math::Clamp(r * scalar, 0.0f, 255.0f)),
        static_cast<uint8_t>(Math::Clamp(g * scalar, 0.0f, 255.0f)),
        static_cast<uint8_t>(Math::Clamp(b * scalar, 0.0f, 255.0f)),
        a
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
        static_cast<uint8_t>((r * other.r) * FACTOR),
        static_cast<uint8_t>((g * other.g) * FACTOR),
        static_cast<uint8_t>((b * other.b) * FACTOR),
        static_cast<uint8_t>((a * other.a) * FACTOR)
    );
}

ColorByte& ColorByte::operator *=(const ColorByte& other)
{
    *this = this->operator*(other);
    return *this;
}

ColorByte ColorByte::operator /(const ColorByte& other) const
{
    float r0 = other.r != 0 ? Math::Clamp01(r / static_cast<float>(other.r)) : 1.0f;
    float g0 = other.g != 0 ? Math::Clamp01(g / static_cast<float>(other.g)) : 1.0f;
    float b0 = other.b != 0 ? Math::Clamp01(b / static_cast<float>(other.b)) : 1.0f;
    float a0 = other.a != 0 ? Math::Clamp01(a / static_cast<float>(other.a)) : 1.0f;

    return ColorByte(
        static_cast<uint8_t>(r0 * 255.0f),
        static_cast<uint8_t>(g0 * 255.0f),
        static_cast<uint8_t>(b0 * 255.0f),
        static_cast<uint8_t>(a0 * 255.0f)
    );
}

ColorByte& ColorByte::operator /=(const ColorByte& other)
{
    *this = this->operator/(other);
    return *this;
}

ColorByte::operator const uint8_t*() const
{
    return &r;
}

ColorByte::operator Color() const
{
    const float NORMALIZE_FACTOR = 1.0f / 255.0f;
    return Color(
        r * NORMALIZE_FACTOR,
        g * NORMALIZE_FACTOR,
        b * NORMALIZE_FACTOR,
        a * NORMALIZE_FACTOR
    );
}

uint8_t& ColorByte::operator [](int index)
{
    switch (index)
    {
        case 0:
            return r;
        case 1:
            return g;
        case 2:
            return b;
        case 3:
            return a;
        default:
            throw "Index out of range";
    }
}
