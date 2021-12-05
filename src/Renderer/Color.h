#ifndef COLOR_H
#define COLOR_H

#include <iostream>


struct Color
{
    Color();
    Color(float r, float g, float b);
    Color(float r, float g, float b, float a);
    Color(const float* arr);

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
    Color operator -() const;
    operator const float* () const;
    float& operator [](int index);

    static Color FromBytes(uint8_t r, uint8_t g, uint8_t b) { return FromBytes(r, g, b, 255); }
    static Color FromBytes(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); }
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

#endif // COLOR_H
