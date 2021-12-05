#ifndef QUATERNION_H
#define QUATERNION_H

#include <algorithm>
#include <iostream>

#include "Convert.h"
#include "Math.h"
#include "Vector.h"

using std::cout;
using std::endl;

struct Quaternion
{
    Quaternion();
    Quaternion(float x, float y, float z, float w);
    Quaternion(const float* arr);

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
    inline Quaternion getNormalized() const { return *this / getMagnitude(); }
    inline void normalize();
    inline Vector3 getEulerAnglesRadians();
    inline Vector3 getEulerAnglesDegrees();

    Quaternion operator *(float scalar) const;
    Quaternion& operator *=(float scalar);
    Quaternion operator /(float divisor) const;
    Quaternion& operator /=(float divisor);
    Quaternion operator *(const Quaternion& other) const;
    Quaternion& operator *=(const Quaternion& other);
    operator const float* () const;
    float& operator [](int index);

    static Quaternion identity;

    static float Dot(const Quaternion& a, const Quaternion& b);
    static Quaternion EulerRadians(const Vector3& radians);
    static Quaternion EulerRadians(float xTheta, float y, float z);
    static Quaternion EulerDegrees(const Vector3& degrees);
    static Quaternion EulerDegrees(float x, float y, float z);

    friend std::ostream& operator<<(std::ostream& os, const Quaternion& v)
    {
        os << '(' << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ')';
        return os;
    }
};

inline void Quaternion::normalize()
{
    float oneOverMagnitude = 1.0f / getMagnitude();
    operator *=(oneOverMagnitude);
}

inline Vector3 Quaternion::getEulerAnglesRadians()
{
    const float EPSILON = 0.000001f;

    float xw = 2.0f * values[0] * values[3];
    float yw = 2.0f * values[1] * values[3];
    float zw = 2.0f * values[2] * values[3];

    float xy = 2.0f * values[0] * values[1];
    float xz = 2.0f * values[0] * values[2];
    float yz = 2.0f * values[1] * values[2];

    float xx = values[0] * values[0];
    float yy = values[1] * values[1];
    float zz = values[2] * values[2];

    float ww = 2.0f * values[3] * values[3];

    // var d1 = qv * qv.wwww * new float4(2.0f); //xw, yw, zw, ww
    // var d2 = qv * qv.yzxw * new float4(2.0f); //xy, yz, zx, ww
    // var d3 = qv * qv; // squares

    const float CUTOFF = (1.0f - 2.0f * EPSILON) * (1.0f - 2.0f * EPSILON);

    float y1 = yz + xw;
    if (y1 * y1 < CUTOFF)
    {
        float x1 = -xz + yw;
        float x2 = zz + ww - xx - yy;
        float z1 = -xy + zw;
        float z2 = yy + ww - zz - xx;
        return Vector3(WrapAngle(asin(y1)), WrapAngle(atan2(x1, x2)), WrapAngle(atan2(z1, z2)));
    }
    else
    {
        y1 = std::clamp(y1, -1.0f, 1.0f);
        // var abcd = new float4(d2.x, d1.z, d2.y, d1.x);
        // var x1 = 2.0f * (abcd.x * abcd.w + abcd.y * abcd.z); //2(ad+bc)
        // var x2 = math.csum(abcd * abcd * new float4(-1.0f, 1.0f, -1.0f, 1.0f));
        // return Vector3(asin(y1), atan2(x1, x2), 0.0f);
        cout << "AH" << endl;
        return Vector3::zero;
    }
}

inline Vector3 Quaternion::getEulerAnglesDegrees()
{
    return rotationToDeg(getEulerAnglesRadians());
}

#endif // QUATERNION_H
