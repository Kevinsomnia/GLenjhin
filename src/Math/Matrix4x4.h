#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <algorithm>
#include <iostream>
#include "Math.h"
#include "Vector.h"

using std::endl;

struct Matrix4x4
{
	float values[16];

	Matrix4x4();
	inline float get(int row, int column) const;
	inline Vector4 getRow(int row) const;
	inline Vector4 getColumn(int column) const;
	inline void set(int row, int column, float value);
	inline void setRow(int row, const Vector4& val);
	inline void setColumn(int column, const Vector4& val);

	operator const float* () const;
	Matrix4x4 operator *(const Matrix4x4& other) const;

	static Matrix4x4 Identity();
	static Matrix4x4 Translate(const Vector3& pos);
	static Matrix4x4 Rotate(const Vector3& rot);
	static Matrix4x4 Scale(const Vector3& scale);
	static Matrix4x4 TRS(const Vector3& pos, const Vector3& rot, const Vector3& scale);

	friend std::ostream& operator<<(std::ostream& os, const Matrix4x4& mat)
	{
		os << mat.get(0, 0) << '\t' << mat.get(0, 1) << '\t' << mat.get(0, 2) << '\t' << mat.get(0, 3) << endl;
		os << mat.get(1, 0) << '\t' << mat.get(1, 1) << '\t' << mat.get(1, 2) << '\t' << mat.get(1, 3) << endl;
		os << mat.get(2, 0) << '\t' << mat.get(2, 1) << '\t' << mat.get(2, 2) << '\t' << mat.get(2, 3) << endl;
		os << mat.get(3, 0) << '\t' << mat.get(3, 1) << '\t' << mat.get(3, 2) << '\t' << mat.get(3, 3);
		return os;
	}
};

#endif // MATRIX4X4_H
