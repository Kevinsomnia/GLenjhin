#pragma once

#include <iostream>


// Forward declarations
struct Vector2;
struct Vector3;
struct Vector4;


// ===========================================================================
// Vector2
// ===========================================================================

struct Vector2
{
    Vector2();
    Vector2(float x, float y);
    Vector2(const float* arr);

    float x;
    float y;

    inline float getMagnitude() const { return sqrt(getSqrMagnitude()); }
    inline float getSqrMagnitude() const { return x * x + y * y; }
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
    operator Vector3() const;
    operator Vector4() const;
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
        os << '(' << v.x << ", " << v.y << ')';
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

    float x;
    float y;
    float z;

    inline float getMagnitude() const { return sqrt(getSqrMagnitude()); }
    inline float getSqrMagnitude() const { return x * x + y * y + z * z; }
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
    operator Vector2() const;
    operator Vector4() const;
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
        os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
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

    float x;
    float y;
    float z;
    float w;

    inline float getMagnitude() const { return sqrt(getSqrMagnitude()); }
    inline float getSqrMagnitude() const { return x * x + y * y + z * z + w * w; }
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
    operator Vector2() const;
    operator Vector3() const;
    float& operator [](int index);

    static Vector4 zero;
    static Vector4 one;

    static float Dot(const Vector4& a, const Vector4& b);

    friend std::ostream& operator<<(std::ostream& os, const Vector4& v)
    {
        os << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';
        return os;
    }
};

inline void Vector4::normalize()
{
    float oneOverMagnitude = 1.0f / getMagnitude();
    operator *=(oneOverMagnitude);
}
