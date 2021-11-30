#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

// ===========================================================================
// Vector2
// ===========================================================================

struct Vector2
{
    Vector2();
    Vector2(float x, float y);
    Vector2(const float* arr);

    float values[2];

    inline float getX() const { return values[0]; }
    inline float getY() const { return values[1]; }
    inline void setX(float x) { values[0] = x; }
    inline void setY(float y) { values[1] = y; }
    inline float getMagnitude() const { return sqrt(getSqrMagnitude()); }
    inline float getSqrMagnitude() const { return values[0] * values[0] + values[1] * values[1]; }
    inline Vector2 getNormalized() const { return *this / getMagnitude(); }
    inline void normalize();

    Vector2 operator +(const Vector2& other) const;
    Vector2& operator +=(const Vector2& other);
    Vector2 operator -(const Vector2& other) const;
    Vector2& operator -=(const Vector2& other);
    Vector2 operator *(float scalar) const;
    Vector2& operator *=(float scalar);
    Vector2 operator /(float divisor) const;
    Vector2& operator /=(float divisor);
    Vector2 operator *(const Vector2& other) const;
    Vector2& operator *=(const Vector2& other);
    Vector2 operator /(const Vector2& other) const;
    Vector2& operator /=(const Vector2& other);
    Vector2 operator -() const;
    operator const float* () const;
    float& operator [](int index);

    static Vector2 zero;
    static Vector2 one;
    static Vector2 left;
    static Vector2 right;
    static Vector2 down;
    static Vector2 up;

    static float Dot(const Vector2& a, const Vector2& b);
    static float Cross(const Vector2& a, const Vector2& b);

    friend std::ostream& operator<<(std::ostream& os, const Vector2& v)
    {
        os << '(' << v[0] << ", " << v[1] << ')';
        return os;
    }
};

inline void Vector2::normalize()
{
    float oneOverMagnitude = 1.0f / getMagnitude();
    operator *=(oneOverMagnitude);
}

// ===========================================================================
// Vector3
// ===========================================================================

struct Vector3
{
    Vector3();
    Vector3(float x, float y, float z);
    Vector3(const float* arr);

    float values[3];

    inline float getX() const { return values[0]; }
    inline float getY() const { return values[1]; }
    inline float getZ() const { return values[2]; }
    inline void setX(float x) { values[0] = x; }
    inline void setY(float y) { values[1] = y; }
    inline void setZ(float z) { values[2] = z; }
    inline float getMagnitude() const { return sqrt(getSqrMagnitude()); }
    inline float getSqrMagnitude() const { return values[0] * values[0] + values[1] * values[1] + values[2] * values[2]; }
    inline Vector3 getNormalized() const { return *this / getMagnitude(); }
    inline void normalize();

    Vector3 operator +(const Vector3& other) const;
    Vector3& operator +=(const Vector3& other);
    Vector3 operator -(const Vector3& other) const;
    Vector3& operator -=(const Vector3& other);
    Vector3 operator *(float scalar) const;
    Vector3& operator *=(float scalar);
    Vector3 operator /(float divisor) const;
    Vector3& operator /=(float divisor);
    Vector3 operator *(const Vector3& other) const;
    Vector3& operator *=(const Vector3& other);
    Vector3 operator /(const Vector3& other) const;
    Vector3& operator /=(const Vector3& other);
    Vector3 operator -() const;
    operator const float* () const;
    float& operator [](int index);

    static Vector3 zero;
    static Vector3 one;
    static Vector3 left;
    static Vector3 right;
    static Vector3 back;
    static Vector3 forward;
    static Vector3 down;
    static Vector3 up;

    static float Dot(const Vector3& a, const Vector3& b);
    static Vector3 Cross(const Vector3& a, const Vector3& b);

    friend std::ostream& operator<<(std::ostream& os, const Vector3& v)
    {
        os << '(' << v[0] << ", " << v[1] << ", " << v[2] << ')';
        return os;
    }
};

inline void Vector3::normalize()
{
    float oneOverMagnitude = 1.0f / getMagnitude();
    operator *=(oneOverMagnitude);
}

// ===========================================================================
// Vector4
// ===========================================================================

struct Vector4
{
    Vector4();
    Vector4(float x, float y, float z, float w);
    Vector4(const float* arr);

    float values[4];

    inline float getX() const { return values[0]; }
    inline float getY() const { return values[1]; }
    inline float getZ() const { return values[2]; }
    inline float getW() const { return values[3]; }
    inline void setX(float x) { values[0] = x; }
    inline void setY(float y) { values[1] = y; }
    inline void setZ(float z) { values[2] = z; }
    inline void setW(float w) { values[3] = w; }
    inline float getMagnitude() const { return sqrt(getSqrMagnitude()); }
    inline float getSqrMagnitude() const { return values[0] * values[0] + values[1] * values[1] + values[2] * values[2] + values[3] * values[3]; }
    inline Vector4 getNormalized() const { return *this / getMagnitude(); }
    inline void normalize();

    Vector4 operator +(const Vector4& other) const;
    Vector4& operator +=(const Vector4& other);
    Vector4 operator -(const Vector4& other) const;
    Vector4& operator -=(const Vector4& other);
    Vector4 operator *(float scalar) const;
    Vector4& operator *=(float scalar);
    Vector4 operator /(float divisor) const;
    Vector4& operator /=(float divisor);
    Vector4 operator *(const Vector4& other) const;
    Vector4& operator *=(const Vector4& other);
    Vector4 operator /(const Vector4& other) const;
    Vector4& operator /=(const Vector4& other);
    Vector4 operator -() const;
    operator const float* () const;
    float& operator [](int index);

    static Vector4 zero;
    static Vector4 one;

    static float Dot(const Vector4& a, const Vector4& b);

    friend std::ostream& operator<<(std::ostream& os, const Vector4& v)
    {
        os << '(' << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ')';
        return os;
    }
};

inline void Vector4::normalize()
{
    float oneOverMagnitude = 1.0f / getMagnitude();
    operator *=(oneOverMagnitude);
}

#endif // VECTOR_H
