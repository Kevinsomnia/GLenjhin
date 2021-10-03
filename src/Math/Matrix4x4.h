#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <algorithm>
#include <iostream>
#include "Math.h"
#include "Vector.h"

using std::endl;

struct Matrix4x4
{
public:
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

inline uint32_t Matrix4x4::getIndex(int row, int column) const
{
	return column * 4 + row;
}

#endif // MATRIX4X4_H
