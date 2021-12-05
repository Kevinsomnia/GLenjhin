#include "Quaternion.h"

Quaternion::Quaternion() : Quaternion(0.0f, 0.0f, 0.0f, 0.0f) { }
Quaternion::Quaternion(float x, float y, float z, float w)
{
    values[0] = x;
    values[1] = y;
    values[2] = z;
    values[3] = w;
}

Quaternion::Quaternion(const float* arr)
{
    for (int i = 0; i < 4; i++)
        values[i] = arr[i];
}

Quaternion Quaternion::operator *(float scalar) const
{
    return Quaternion(values[0] * scalar, values[1] * scalar, values[2] * scalar, values[3] * scalar);
}

Quaternion& Quaternion::operator *=(float scalar)
{
    *this = this->operator*(scalar);
    return *this;
}

Quaternion Quaternion::operator /(float divisor) const
{
    float scalar = 1.0f / divisor;
    return Quaternion(values[0] * scalar, values[1] * scalar, values[2] * scalar, values[3] * scalar);
}

Quaternion& Quaternion::operator /=(float divisor)
{
    *this = this->operator/(divisor);
    return *this;
}

Quaternion Quaternion::operator *(const Quaternion& other) const
{
    // "Adds" two Quaternions together
    return Quaternion(
        values[3] * other.values[0] + values[0] * other.values[3] + values[1] * other.values[2] - values[2] * other.values[1],
        values[3] * other.values[1] + values[1] * other.values[3] + values[2] * other.values[0] - values[0] * other.values[2],
        values[3] * other.values[2] + values[2] * other.values[3] + values[0] * other.values[1] - values[1] * other.values[0],
        values[3] * other.values[3] - values[0] * other.values[0] - values[1] * other.values[1] - values[2] * other.values[2]
    );
}

Quaternion& Quaternion::operator *=(const Quaternion& other)
{
    float x = values[3] * other.values[0] + values[0] * other.values[3] + values[1] * other.values[2] - values[2] * other.values[1];
    float y =    values[3] * other.values[1] + values[1] * other.values[3] + values[2] * other.values[0] - values[0] * other.values[2];
     float z =   values[3] * other.values[2] + values[2] * other.values[3] + values[0] * other.values[1] - values[1] * other.values[0];
     float w =   values[3] * other.values[3] - values[0] * other.values[0] - values[1] * other.values[1] - values[2] * other.values[2];
    values[0] = x;
    values[1] = y;
    values[2] = z;
    values[3] = w;
    return *this;
}

Quaternion::operator const float*() const
{
    return values;
}

float& Quaternion::operator [](int index)
{
    return values[index];
}

Quaternion Quaternion::identity = Quaternion();

float Quaternion::Dot(const Quaternion& a, const Quaternion& b)
{
    return a.values[0] * b.values[0] + a.values[1] * b.values[1] + a.values[2] * b.values[2] + a.values[3] * b.values[3];
}

Quaternion Quaternion::EulerRadians(const Vector3& radians)
{
    return Quaternion::EulerRadians(radians.getX(), radians.getY(), radians.getZ());
}

Quaternion Quaternion::EulerRadians(float xTheta, float yTheta, float zTheta)
{
    xTheta = WrapAngle(xTheta);
    yTheta = WrapAngle(yTheta);
    zTheta = WrapAngle(zTheta);
    float cx = cos(xTheta * 0.5f);
    float sx = sin(xTheta * 0.5f);
    float cy = cos(yTheta * 0.5f);
    float sy = sin(yTheta * 0.5f);
    float cz = cos(zTheta * 0.5f);
    float sz = sin(zTheta * 0.5f);

    return Quaternion(
        sx * cy * cz - cx * sy * sz,
        cx * sy * cz + sx * cy * sz,
        cx * cy * sz - sx * sy * cz,
        cx * cy * cz + sx * sy * sz
    );
}

Quaternion Quaternion::EulerDegrees(const Vector3& degrees)
{
    Vector3 radians = rotationToRad(degrees);
    return Quaternion::EulerRadians(radians.getX(), radians.getY(), radians.getZ());
}

Quaternion Quaternion::EulerDegrees(float x, float y, float z)
{
    return Quaternion::EulerRadians(degToRad(x), degToRad(y), degToRad(z));
}
