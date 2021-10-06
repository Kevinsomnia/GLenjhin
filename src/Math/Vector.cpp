#include "Vector.h"

// ===========================================================================
// Vector2
// ===========================================================================

Vector2::Vector2() : Vector2(0.0f, 0.0f) { }
Vector2::Vector2(float x, float y)
{
    values[0] = x;
    values[1] = y;
}

Vector2::Vector2(const float* arr)
{
    for (int i = 0; i < 2; i++)
        values[i] = arr[i];
}

Vector2 Vector2::operator +(const Vector2& other) const
{
    return Vector2(values[0] + other.values[0], values[1] + other.values[1]);
}

Vector2& Vector2::operator +=(const Vector2& other)
{
    *this = this->operator+(other);
    return *this;
}

Vector2 Vector2::operator -(const Vector2& other) const
{
    return Vector2(values[0] - other.values[0], values[1] - other.values[1]);
}

Vector2& Vector2::operator -=(const Vector2& other)
{
    *this = this->operator-(other);
    return *this;
}

Vector2 Vector2::operator *(float scalar) const
{
    return Vector2(values[0] * scalar, values[1] * scalar);
}

Vector2& Vector2::operator *=(float scalar)
{
    *this = this->operator*(scalar);
    return *this;
}

Vector2 Vector2::operator /(float divisor) const
{
    float scalar = 1.0f / divisor;
    return Vector2(values[0] * scalar, values[1] * scalar);
}

Vector2& Vector2::operator /=(float divisor)
{
    *this = this->operator/(divisor);
    return *this;
}

Vector2 Vector2::operator *(const Vector2& other) const
{
    return Vector2(values[0] * other.values[0], values[1] * other.values[1]);
}

Vector2& Vector2::operator *=(const Vector2& other)
{
    *this = this->operator*(other);
    return *this;
}

Vector2 Vector2::operator /(const Vector2& other) const
{
    return Vector2(values[0] / other.values[0], values[1] / other.values[1]);
}

Vector2& Vector2::operator /=(const Vector2& other)
{
    *this = this->operator/(other);
    return *this;
}

Vector2 Vector2::operator -() const
{
    return Vector2(-values[0], -values[1]);
}

Vector2::operator const float*() const
{
    return values;
}

float& Vector2::operator [](int index)
{
    return values[index];
}

Vector2 Vector2::zero = Vector2();
Vector2 Vector2::one = Vector2(1.0f, 1.0f);
Vector2 Vector2::left = Vector2(-1.0f, 0.0f);
Vector2 Vector2::right = Vector2(1.0f, 0.0f);
Vector2 Vector2::down = Vector2(0.0f, -1.0f);
Vector2 Vector2::up = Vector2(0.0f, 1.0f);

float Vector2::Dot(const Vector2& a, const Vector2& b)
{
    return a.values[0] * b.values[0] + a.values[1] * b.values[1];
}

float Vector2::Cross(const Vector2& a, const Vector2& b)
{
    // Returns the area of the parallelogram formed by a and b.
    return a.values[0] * b.values[1] - a.values[1] * b.values[0];
}

// ===========================================================================
// Vector3
// ===========================================================================

Vector3::Vector3() : Vector3(0.0f, 0.0f, 0.0f) { }
Vector3::Vector3(float x, float y, float z)
{
    values[0] = x;
    values[1] = y;
    values[2] = z;
}

Vector3::Vector3(const float* arr)
{
    for (int i = 0; i < 3; i++)
        values[i] = arr[i];
}

Vector3 Vector3::operator +(const Vector3& other) const
{
    return Vector3(values[0] + other.values[0], values[1] + other.values[1], values[2] + other.values[2]);
}

Vector3& Vector3::operator +=(const Vector3& other)
{
    *this = this->operator+(other);
    return *this;
}

Vector3 Vector3::operator -(const Vector3& other) const
{
    return Vector3(values[0] - other.values[0], values[1] - other.values[1], values[2] - other.values[2]);
}

Vector3& Vector3::operator -=(const Vector3& other)
{
    *this = this->operator-(other);
    return *this;
}

Vector3 Vector3::operator *(float scalar) const
{
    return Vector3(values[0] * scalar, values[1] * scalar, values[2] * scalar);
}

Vector3& Vector3::operator *=(float scalar)
{
    *this = this->operator*(scalar);
    return *this;
}

Vector3 Vector3::operator /(float divisor) const
{
    float scalar = 1.0f / divisor;
    return Vector3(values[0] * scalar, values[1] * scalar, values[2] * scalar);
}

Vector3& Vector3::operator /=(float divisor)
{
    *this = this->operator/(divisor);
    return *this;
}

Vector3 Vector3::operator *(const Vector3& other) const
{
    return Vector3(values[0] * other.values[0], values[1] * other.values[1], values[2] * other.values[2]);
}

Vector3& Vector3::operator *=(const Vector3& other)
{
    *this = this->operator*(other);
    return *this;
}

Vector3 Vector3::operator /(const Vector3& other) const
{
    return Vector3(values[0] / other.values[0], values[1] / other.values[1], values[2] / other.values[2]);
}

Vector3& Vector3::operator /=(const Vector3& other)
{
    *this = this->operator/(other);
    return *this;
}

Vector3 Vector3::operator -() const
{
    return Vector3(-values[0], -values[1], -values[2]);
}

Vector3::operator const float*() const
{
    return values;
}

float& Vector3::operator [](int index)
{
    return values[index];
}

Vector3 Vector3::zero = Vector3();
Vector3 Vector3::one = Vector3(1.0f, 1.0f, 1.0f);
Vector3 Vector3::left = Vector3(-1.0f, 0.0f, 0.0f);
Vector3 Vector3::right = Vector3(1.0f, 0.0f, 0.0f);
Vector3 Vector3::back = Vector3(0.0f, 0.0f, -1.0f);
Vector3 Vector3::forward = Vector3(0.0f, 0.0f, 1.0f);
Vector3 Vector3::down = Vector3(0.0f, -1.0f, 0.0f);
Vector3 Vector3::up = Vector3(0.0f, 1.0f, 0.0f);

float Vector3::Dot(const Vector3& a, const Vector3& b)
{
    return a.values[0] * b.values[0] + a.values[1] * b.values[1] + a.values[2] * b.values[2];
}

Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
{
    return Vector3(
        a.values[1] * b.values[2] - a.values[2] * b.values[1],
        a.values[2] * b.values[0] - a.values[0] * b.values[2],
        a.values[0] * b.values[1] - a.values[1] * b.values[0]
    );
}

// ===========================================================================
// Vector4
// ===========================================================================

Vector4::Vector4() : Vector4(0.0f, 0.0f, 0.0f, 0.0f) { }
Vector4::Vector4(float x, float y, float z, float w)
{
    values[0] = x;
    values[1] = y;
    values[2] = z;
    values[3] = w;
}

Vector4::Vector4(const float* arr)
{
    for (int i = 0; i < 4; i++)
        values[i] = arr[i];
}

Vector4 Vector4::operator +(const Vector4& other) const
{
    return Vector4(values[0] + other.values[0], values[1] + other.values[1], values[2] + other.values[2], values[3] + other.values[3]);
}

Vector4& Vector4::operator +=(const Vector4& other)
{
    *this = this->operator+(other);
    return *this;
}

Vector4 Vector4::operator -(const Vector4& other) const
{
    return Vector4(values[0] - other.values[0], values[1] - other.values[1], values[2] + other.values[2], values[3] + other.values[3]);
}

Vector4& Vector4::operator -=(const Vector4& other)
{
    *this = this->operator-(other);
    return *this;
}

Vector4 Vector4::operator *(float scalar) const
{
    return Vector4(values[0] * scalar, values[1] * scalar, values[2] * scalar, values[3] * scalar);
}

Vector4& Vector4::operator *=(float scalar)
{
    *this = this->operator*(scalar);
    return *this;
}

Vector4 Vector4::operator /(float divisor) const
{
    float scalar = 1.0f / divisor;
    return Vector4(values[0] * scalar, values[1] * scalar, values[2] * scalar, values[3] * scalar);
}

Vector4& Vector4::operator /=(float divisor)
{
    *this = this->operator/(divisor);
    return *this;
}

Vector4 Vector4::operator *(const Vector4& other) const
{
    return Vector4(values[0] * other.values[0], values[1] * other.values[1], values[2] * other.values[2], values[3] * other.values[3]);
}

Vector4& Vector4::operator *=(const Vector4& other)
{
    *this = this->operator*(other);
    return *this;
}

Vector4 Vector4::operator /(const Vector4& other) const
{
    return Vector4(values[0] / other.values[0], values[1] / other.values[1], values[2] / other.values[2], values[3] / other.values[3]);
}

Vector4& Vector4::operator /=(const Vector4& other)
{
    *this = this->operator/(other);
    return *this;
}

Vector4 Vector4::operator -() const
{
    return Vector4(-values[0], -values[1], -values[2], -values[3]);
}

Vector4::operator const float*() const
{
    return values;
}

float& Vector4::operator [](int index)
{
    return values[index];
}

Vector4 Vector4::zero = Vector4();
Vector4 Vector4::one = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

float Vector4::Dot(const Vector4& a, const Vector4& b)
{
    return a.values[0] * b.values[0] + a.values[1] * b.values[1] + a.values[2] * b.values[2] + a.values[3] * b.values[3];
}
