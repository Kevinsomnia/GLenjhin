#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "../Math/Math.h"


// Forward declarations
struct ColorByte;


struct Color
{
    Color();
    Color(float r, float g, float b);
    Color(float r, float g, float b, float a);
    Color(const float* arr, bool alpha = true);

    float values[4];

    inline float getR() const { return values[0]; }
    inline float getG() const { return values[1]; }
    inline float getB() const { return values[2]; }
    inline float getA() const { return values[3]; }
    inline void setR(float r) { values[0] = r; }
    inline void setG(float g) { values[1] = g; }
    inline void setB(float b) { values[2] = b; }
    inline void setA(float a) { values[3] = a; }

    Color operator +(const Color& other) const;
    Color& operator +=(const Color& other);
    Color operator -(const Color& other) const;
    Color& operator -=(const Color& other);
    Color operator *(float scalar) const;
    Color& operator *=(float scalar);
    Color operator /(float divisor) const;
    Color& operator /=(float divisor);
    Color operator *(const Color& other) const;
    Color& operator *=(const Color& other);
    Color operator /(const Color& other) const;
    Color& operator /=(const Color& other);
    operator const float* () const;
    operator ColorByte () const;
    float& operator [](int index);

    static Color Clear() { return Color(0.0f, 0.0f, 0.0f, 0.0f); }
    static Color Black() { return Color(0.0f, 0.0f, 0.0f); }
    static Color Gray() { return Color(0.5f, 0.5f, 0.5f); }
    static Color White() { return Color(1.0f, 1.0f, 1.0f); }
    static Color Red() { return Color(1.0f, 0.0f, 0.0f); }
    static Color Green() { return Color(0.0f, 1.0f, 0.0f); }
    static Color Blue() { return Color(0.0f, 0.0f, 1.0f); }
    static Color Azure() { return Color(0.0f, 0.5f, 1.0f); }
    static Color Yellow() { return Color(1.0f, 1.0f, 0.0f); }
    static Color Cyan() { return Color(0.0f, 1.0f, 1.0f); }
    static Color Magenta() { return Color(1.0f, 0.0f, 1.0f); }
    static Color Pink() { return Color(1.0f, 0.0f, 0.5f); }
    static Color Orange() { return Color(1.0f, 0.5f, 0.0f); }
    static Color Purple() { return Color(0.5f, 0.0f, 1.0f); }

    friend std::ostream& operator<<(std::ostream& os, const Color& v)
    {
        os << '(' << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ')';
        return os;
    }
};

struct ColorByte
{
    ColorByte();
    ColorByte(uint8_t r, uint8_t g, uint8_t b);
    ColorByte(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    ColorByte(const uint8_t* arr, bool alpha = true);

    uint8_t values[4];

    inline uint8_t getR() const { return values[0]; }
    inline uint8_t getG() const { return values[1]; }
    inline uint8_t getB() const { return values[2]; }
    inline uint8_t getA() const { return values[3]; }
    inline void setR(uint8_t r) { values[0] = r; }
    inline void setG(uint8_t g) { values[1] = g; }
    inline void setB(uint8_t b) { values[2] = b; }
    inline void setA(uint8_t a) { values[3] = a; }

    ColorByte operator +(const ColorByte& other) const;
    ColorByte& operator +=(const ColorByte& other);
    ColorByte operator -(const ColorByte& other) const;
    ColorByte& operator -=(const ColorByte& other);
    ColorByte operator *(float scalar) const;
    ColorByte& operator *=(float scalar);
    ColorByte operator /(float divisor) const;
    ColorByte& operator /=(float divisor);
    ColorByte operator *(const ColorByte& other) const;
    ColorByte& operator *=(const ColorByte& other);
    ColorByte operator /(const ColorByte& other) const;
    ColorByte& operator /=(const ColorByte& other);
    operator const uint8_t* () const;
    operator Color () const;
    uint8_t& operator [](int index);

    static ColorByte Clear() { return ColorByte(0, 0, 0, 0); }
    static ColorByte Black() { return ColorByte(0, 0, 0); }
    static ColorByte Gray() { return ColorByte(128, 128, 128); }
    static ColorByte White() { return ColorByte(255, 255, 255); }
    static ColorByte Red() { return ColorByte(255, 0, 0); }
    static ColorByte Green() { return ColorByte(0, 255, 0); }
    static ColorByte Blue() { return ColorByte(0, 0, 255); }
    static ColorByte Azure() { return ColorByte(0, 128, 255); }
    static ColorByte Yellow() { return ColorByte(255, 255, 0); }
    static ColorByte Cyan() { return ColorByte(0, 255, 255); }
    static ColorByte Magenta() { return ColorByte(255, 0, 255); }
    static ColorByte Pink() { return ColorByte(255, 0, 128); }
    static ColorByte Orange() { return ColorByte(255, 128, 0); }
    static ColorByte Purple() { return ColorByte(128, 0, 255); }

    friend std::ostream& operator<<(std::ostream& os, const ColorByte& v)
    {
        os << '(' << static_cast<uint16_t>(v[0]) << ", ";
        os << static_cast<uint16_t>(v[1]) << ", ";
        os << static_cast<uint16_t>(v[2]) << ", ";
        os << static_cast<uint16_t>(v[3]) << ')';
        return os;
    }
};

#endif // COLOR_H
