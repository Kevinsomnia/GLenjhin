#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <algorithm>
#include <iostream>
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
	int start = column * 4;
	return Vector4(values[start], values[start + 1], values[start + 2], values[start + 3]);
}

inline void Matrix4x4::set(int row, int column, float value)
{
	values[getIndex(row, column)] = value;
}

inline void Matrix4x4::setRow(int row, const Vector4& val)
{
	values[row] = val.x;
	values[row + 4] = val.y;
	values[row + 8] = val.z;
	values[row + 12] = val.w;
}

inline void Matrix4x4::setColumn(int column, const Vector4& val)
{
	int start = column * 4;
	values[start] = val.x;
	values[start + 1] = val.y;
	values[start + 2] = val.z;
	values[start + 3] = val.w;
}

inline Vector3 Matrix4x4::multiplyPoint3x4(const Vector3& p) const
{
	return Vector3(
		p.x * values[0] + p.y * values[4] + p.z * values[8] + values[12],
		p.x * values[1] + p.y * values[5] + p.z * values[9] + values[13],
		p.x * values[2] + p.y * values[6] + p.z * values[10] + values[14]
	);
}

inline Vector3 Matrix4x4::multiplyPoint(const Vector3& p) const
{
	// Divide (x,y,z) by perspective component (w)
	float oneOverW = 1.0f / (p.x * values[3] + p.y * values[7] + p.z * values[11] + values[15]);

	return Vector3(
		(p.x * values[0] + p.y * values[4] + p.z * values[8] + values[12]) * oneOverW,
		(p.x * values[1] + p.y * values[5] + p.z * values[9] + values[13]) * oneOverW,
		(p.x * values[2] + p.y * values[6] + p.z * values[10] + values[14]) * oneOverW
	);
}

inline Vector3 Matrix4x4::multiplyVector(const Vector3& v) const
{
	return Vector3(
		v.x * values[0] + v.y * values[4] + v.z * values[8],
		v.x * values[1] + v.y * values[5] + v.z * values[9],
		v.x * values[2] + v.y * values[6] + v.z * values[10]
	);
}

inline uint32_t Matrix4x4::getIndex(int row, int column) const
{
	return column * 4 + row;
}

#endif // MATRIX4X4_H
