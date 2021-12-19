#include "Vector.h"

// ===========================================================================
// Vector2
// ===========================================================================

Vector2::Vector2() : Vector2(0.0f, 0.0f) { }
Vector2::Vector2(float x, float y) : x(x), y(y) { }
Vector2::Vector2(const float* arr)
{
    x = arr[0];
    y = arr[1];
}

Vector2 Vector2::operator +(const Vector2& other) const
{
    return Vector2(x + other.x, y + other.y);
}

Vector2& Vector2::operator +=(const Vector2& other)
{
    *this = this->operator+(other);
    return *this;
}

Vector2 Vector2::operator -(const Vector2& other) const
{
    return Vector2(x - other.x, y - other.y);
}

Vector2& Vector2::operator -=(const Vector2& other)
{
    *this = this->operator-(other);
    return *this;
}

Vector2 Vector2::operator *(float scalar) const
{
    return Vector2(x * scalar, y * scalar);
}

Vector2& Vector2::operator *=(float scalar)
{
    *this = this->operator*(scalar);
    return *this;
}

Vector2 Vector2::operator /(float divisor) const
{
    float scalar = 1.0f / divisor;
    return Vector2(x * scalar, y * scalar);
}

Vector2& Vector2::operator /=(float divisor)
{
    *this = this->operator/(divisor);
    return *this;
}

Vector2 Vector2::operator *(const Vector2& other) const
{
    return Vector2(x * other.x, y * other.y);
}

Vector2& Vector2::operator *=(const Vector2& other)
{
    *this = this->operator*(other);
    return *this;
}

Vector2 Vector2::operator /(const Vector2& other) const
{
    return Vector2(x / other.x, y / other.y);
}

Vector2& Vector2::operator /=(const Vector2& other)
{
    *this = this->operator/(other);
    return *this;
}

Vector2 Vector2::operator -() const
{
    return Vector2(-x, -y);
}

Vector2::operator const float*() const
{
    return &x;
}

Vector2::operator Vector3() const
{
    return Vector3(x, y, 0.0f);
}

Vector2::operator Vector4() const
{
    return Vector4(x, y, 0.0f, 0.0f);
}

float& Vector2::operator [](int index)
{
    switch (index)
    {
        case 0:
            return x;
        case 1:
            return y;
        default:
            throw "Index out of range";
    }
}

Vector2 Vector2::zero = Vector2();
Vector2 Vector2::one = Vector2(1.0f, 1.0f);
Vector2 Vector2::left = Vector2(-1.0f, 0.0f);
Vector2 Vector2::right = Vector2(1.0f, 0.0f);
Vector2 Vector2::down = Vector2(0.0f, -1.0f);
Vector2 Vector2::up = Vector2(0.0f, 1.0f);

float Vector2::Dot(const Vector2& a, const Vector2& b)
{
    return a.x * b.x + a.x * b.x;
}

float Vector2::Cross(const Vector2& a, const Vector2& b)
{
    // Returns the area of the parallelogram formed by a and b.
    return a.x * b.y - a.y * b.x;
}

// ===========================================================================
// Vector3
// ===========================================================================

Vector3::Vector3() : Vector3(0.0f, 0.0f, 0.0f) { }
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) { }

Vector3::Vector3(const float* arr)
{
    x = arr[0];
    y = arr[1];
    z = arr[2];
}

Vector3 Vector3::operator +(const Vector3& other) const
{
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3& Vector3::operator +=(const Vector3& other)
{
    *this = this->operator+(other);
    return *this;
}

Vector3 Vector3::operator -(const Vector3& other) const
{
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3& Vector3::operator -=(const Vector3& other)
{
    *this = this->operator-(other);
    return *this;
}

Vector3 Vector3::operator *(float scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3& Vector3::operator *=(float scalar)
{
    *this = this->operator*(scalar);
    return *this;
}

Vector3 Vector3::operator /(float divisor) const
{
    float scalar = 1.0f / divisor;
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3& Vector3::operator /=(float divisor)
{
    *this = this->operator/(divisor);
    return *this;
}

Vector3 Vector3::operator *(const Vector3& other) const
{
    return Vector3(x * other.x, y * other.y, z * other.z);
}

Vector3& Vector3::operator *=(const Vector3& other)
{
    *this = this->operator*(other);
    return *this;
}

Vector3 Vector3::operator /(const Vector3& other) const
{
    return Vector3(x / other.x, y / other.y, z / other.z);
}

Vector3& Vector3::operator /=(const Vector3& other)
{
    *this = this->operator/(other);
    return *this;
}

Vector3 Vector3::operator -() const
{
    return Vector3(-x, -y, -z);
}

Vector3::operator const float*() const
{
    return &x;
}

Vector3::operator Vector2() const
{
    return Vector2(x, y);
}

Vector3::operator Vector4() const
{
    return Vector4(x, y, z, 0.0f);
}

float& Vector3::operator [](int index)
{
    switch (index)
    {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            throw "Index out of range";
    }
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
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
{
    return Vector3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

// ===========================================================================
// Vector4
// ===========================================================================

Vector4::Vector4() : Vector4(0.0f, 0.0f, 0.0f, 0.0f) { }
Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

Vector4::Vector4(const float* arr)
{
    x = arr[0];
    y = arr[1];
    z = arr[2];
    w = arr[3];
}

Vector4 Vector4::operator +(const Vector4& other) const
{
    return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4& Vector4::operator +=(const Vector4& other)
{
    *this = this->operator+(other);
    return *this;
}

Vector4 Vector4::operator -(const Vector4& other) const
{
    return Vector4(x - other.x, y - other.y, z + other.z, w + other.w);
}

Vector4& Vector4::operator -=(const Vector4& other)
{
    *this = this->operator-(other);
    return *this;
}

Vector4 Vector4::operator *(float scalar) const
{
    return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vector4& Vector4::operator *=(float scalar)
{
    *this = this->operator*(scalar);
    return *this;
}

Vector4 Vector4::operator /(float divisor) const
{
    float scalar = 1.0f / divisor;
    return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vector4& Vector4::operator /=(float divisor)
{
    *this = this->operator/(divisor);
    return *this;
}

Vector4 Vector4::operator *(const Vector4& other) const
{
    return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
}

Vector4& Vector4::operator *=(const Vector4& other)
{
    *this = this->operator*(other);
    return *this;
}

Vector4 Vector4::operator /(const Vector4& other) const
{
    return Vector4(x / other.x, y / other.y, z / other.z, w / other.w);
}

Vector4& Vector4::operator /=(const Vector4& other)
{
    *this = this->operator/(other);
    return *this;
}

Vector4 Vector4::operator -() const
{
    return Vector4(-x, -y, -z, -w);
}

Vector4::operator const float*() const
{
    return &x;
}

Vector4::operator Vector2() const
{
    return Vector2(x, y);
}

Vector4::operator Vector3() const
{
    return Vector3(x, y, z);
}

float& Vector4::operator [](int index)
{
    switch (index)
    {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
        default:
            throw "Index out of range";
    }
}

Vector4 Vector4::zero = Vector4();
Vector4 Vector4::one = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

float Vector4::Dot(const Vector4& a, const Vector4& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
