#ifndef COLOR_H
#define COLOR_H

#include <iostream>


struct Color
{
    Color();
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

    static Color clear() { return Color(0.0f, 0.0f, 0.0f, 0.0f); }
    static Color black() { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
    static Color gray() { return Color(0.5f, 0.5f, 0.5f, 1.0f); }
    static Color white() { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
    static Color red() { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
    static Color green() { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
    static Color blue() { return Color(0.0f, 0.0f, 1.0f, 1.0f); }
    static Color azure() { return Color(0.0f, 0.5f, 1.0f, 1.0f); }
    static Color yellow() { return Color(1.0f, 1.0f, 0.0f, 1.0f); }
    static Color cyan() { return Color(0.0f, 1.0f, 1.0f, 1.0f); }
    static Color magenta() { return Color(1.0f, 0.0f, 1.0f, 1.0f); }
    static Color pink() { return Color(1.0f, 0.0f, 0.5f, 1.0f); }
    static Color orange() { return Color(1.0f, 0.5f, 0.0f, 1.0f); }
    static Color purple() { return Color(0.5f, 0.0f, 1.0f, 1.0f); }

    friend std::ostream& operator<<(std::ostream& os, const Color& v)
    {
        os << '(' << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ')';
        return os;
    }
};

#endif // COLOR_H
