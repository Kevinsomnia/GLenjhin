#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <algorithm>
#include "Convert.h"
#include "Math.h"
#include "Vector.h"

using std::cout;
using std::endl;

struct Matrix4x4
{
public:
    float values[16];

    Matrix4x4();
    Matrix4x4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3);

    inline float get(int row, int column) const;
    inline Vector4 getRow(int row) const;
    inline Vector4 getColumn(int column) const;
    inline void set(int row, int column, float value);
    inline void setRow(int row, const Vector4& val);
    inline void setColumn(int column, const Vector4& val);
    inline Vector3 multiplyPoint3x4(const Vector3& p) const;
    inline Vector3 multiplyPoint(const Vector3& p) const;
    inline Vector3 multiplyVector(const Vector3& v) const;

    operator const float* () const;
    Matrix4x4 operator *(const Matrix4x4& other) const;

    static Matrix4x4 identity;

    static Matrix4x4 Translate(const Vector3& pos);
    static Matrix4x4 Rotate(const Vector3& rot);
    static Matrix4x4 Scale(const Vector3& scale);
    static Matrix4x4 TRS(const Vector3& pos, const Vector3& rot, const Vector3& scale);
    static Matrix4x4 View(const Vector3& pos, const Vector3& rot);
    static Matrix4x4 Orthographic(float size, float aspect, float near, float far);
    static Matrix4x4 Perspective(float fov, float aspect, float near, float far);

    friend std::ostream& operator<<(std::ostream& os, const Matrix4x4& mat)
    {
        os << mat.values[0] << '\t' << mat.values[4] << '\t' << mat.values[8] << '\t' << mat.values[12] << endl;
        os << mat.values[1] << '\t' << mat.values[5] << '\t' << mat.values[9] << '\t' << mat.values[13] << endl;
        os << mat.values[2] << '\t' << mat.values[6] << '\t' << mat.values[10] << '\t' << mat.values[14] << endl;
        os << mat.values[3] << '\t' << mat.values[7] << '\t' << mat.values[11] << '\t' << mat.values[15];
        return os;
    }
private:
    inline uint32_t getIndex(int row, int column) const;
};

inline float Matrix4x4::get(int row, int column) const
{
    return values[getIndex(row, column)];
}

inline Vector4 Matrix4x4::getRow(int row) const
{
    return Vector4(values[row], values[row + 4], values[row + 8], values[row + 12]);
}

inline Vector4 Matrix4x4::getColumn(int column) const
{
    return Vector4(values + (column * 4));
}

inline void Matrix4x4::set(int row, int column, float value)
{
    values[getIndex(row, column)] = value;
}

inline void Matrix4x4::setRow(int row, const Vector4& val)
{
    for (int i = 0; i < 4; i++)
        values[row + (i * 4)] = val.values[i];
}

inline void Matrix4x4::setColumn(int column, const Vector4& val)
{
    int start = column * 4;

    for (int i = 0; i < 4; i++)
        values[start + i] = val.values[i];
}

inline Vector3 Matrix4x4::multiplyPoint3x4(const Vector3& p) const
{
    return Vector3(
        p.getX() * values[0] + p.getY() * values[4] + p.getZ() * values[8] + values[12],
        p.getX() * values[1] + p.getY() * values[5] + p.getZ() * values[9] + values[13],
        p.getX() * values[2] + p.getY() * values[6] + p.getZ() * values[10] + values[14]
    );
}

inline Vector3 Matrix4x4::multiplyPoint(const Vector3& p) const
{
    // Divide (x,y,z) by perspective component (w)
    float oneOverW = 1.0f / (p.getX() * values[3] + p.getY() * values[7] + p.getZ() * values[11] + values[15]);

    return Vector3(
        (p.getX() * values[0] + p.getY() * values[4] + p.getZ() * values[8] + values[12]) * oneOverW,
        (p.getX() * values[1] + p.getY() * values[5] + p.getZ() * values[9] + values[13]) * oneOverW,
        (p.getX() * values[2] + p.getY() * values[6] + p.getZ() * values[10] + values[14]) * oneOverW
    );
}

inline Vector3 Matrix4x4::multiplyVector(const Vector3& v) const
{
    return Vector3(
        v.getX() * values[0] + v.getY() * values[4] + v.getZ() * values[8],
        v.getX() * values[1] + v.getY() * values[5] + v.getZ() * values[9],
        v.getX() * values[2] + v.getY() * values[6] + v.getZ() * values[10]
    );
}

inline uint32_t Matrix4x4::getIndex(int row, int column) const
{
    return column * 4 + row;
}

#endif // MATRIX4X4_H
