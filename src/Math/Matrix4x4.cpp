#include "Matrix4x4.h"

inline uint32_t getIndex(int row, int column)
{
	return (column << 2) + row;
}

Matrix4x4::Matrix4x4()
{
	std::fill(std::begin(values), std::end(values), 0.0f);
}

float Matrix4x4::get(int row, int column) const
{
	return values[getIndex(row, column)];
}

const Vector4& Matrix4x4::getRow(int row) const
{
	return Vector4(get(row, 0), get(row, 1), get(row, 2), get(row, 3));
}

const Vector4& Matrix4x4::getColumn(int column) const
{
	return Vector4(get(0, column), get(1, column), get(2, column), get(3, column));
}

void Matrix4x4::set(int row, int column, float value)
{
	values[getIndex(row, column)] = value;
}

void Matrix4x4::setRow(int row, const Vector4& values)
{
	set(row, 0, values.x);
	set(row, 1, values.y);
	set(row, 2, values.z);
	set(row, 3, values.w);
}

void Matrix4x4::setColumn(int column, const Vector4& values)
{
	set(0, column, values.x);
	set(1, column, values.y);
	set(2, column, values.z);
	set(3, column, values.w);
}

Matrix4x4::operator const float*() const
{
	return values;
}

Matrix4x4 Matrix4x4::operator *(const Matrix4x4& other) const
{
	Vector4 a0 = getRow(0);
	Vector4 a1 = getRow(1);
	Vector4 a2 = getRow(2);
	Vector4 a3 = getRow(3);
	Vector4 b0 = other.getColumn(0);
	Vector4 b1 = other.getColumn(1);
	Vector4 b2 = other.getColumn(2);
	Vector4 b3 = other.getColumn(3);

	Matrix4x4 result;
	result.set(0, 0, Vector4::Dot(a0, b0));
	result.set(0, 1, Vector4::Dot(a0, b1));
	result.set(0, 2, Vector4::Dot(a0, b2));
	result.set(0, 3, Vector4::Dot(a0, b3));
	result.set(1, 0, Vector4::Dot(a1, b0));
	result.set(1, 1, Vector4::Dot(a1, b1));
	result.set(1, 2, Vector4::Dot(a1, b2));
	result.set(1, 3, Vector4::Dot(a1, b3));
	result.set(2, 0, Vector4::Dot(a2, b0));
	result.set(2, 1, Vector4::Dot(a2, b1));
	result.set(2, 2, Vector4::Dot(a2, b2));
	result.set(2, 3, Vector4::Dot(a2, b3));
	result.set(3, 0, Vector4::Dot(a3, b0));
	result.set(3, 1, Vector4::Dot(a3, b1));
	result.set(3, 2, Vector4::Dot(a3, b2));
	result.set(3, 3, Vector4::Dot(a3, b3));
	return result;
}

Matrix4x4 Matrix4x4::Identity()
{
	Matrix4x4 result = Matrix4x4();
	result.set(0, 0, 1.0f);
	result.set(1, 1, 1.0f);
	result.set(2, 2, 1.0f);
	result.set(3, 3, 1.0f);
	return result;
}

Matrix4x4 Matrix4x4::Translate(const Vector3& pos)
{
	Matrix4x4 result = Matrix4x4::Identity();
	result.set(0, 3, pos.x);
	result.set(1, 3, pos.y);
	result.set(2, 3, pos.z);
	return result;
}

Matrix4x4 Matrix4x4::Rotate(const Vector3& rot)
{
	Matrix4x4 matrixX = Matrix4x4();
	matrixX.set(0, 0, 1.0f);
	matrixX.set(1, 1, cos(rot.x));
	matrixX.set(1, 2, -sin(rot.x));
	matrixX.set(2, 1, sin(rot.x));
	matrixX.set(2, 2, cos(rot.x));
	matrixX.set(3, 3, 1.0f);

	Matrix4x4 matrixY = Matrix4x4();
	matrixY.set(0, 0, cos(rot.y));
	matrixY.set(0, 2, sin(rot.y));
	matrixY.set(1, 1, 1.0f);
	matrixY.set(2, 0, -sin(rot.y));
	matrixY.set(2, 2, cos(rot.y));
	matrixY.set(3, 3, 1.0f);

	Matrix4x4 matrixZ = Matrix4x4();
	matrixZ.set(0, 0, cos(rot.z));
	matrixZ.set(0, 1, -sin(rot.z));
	matrixZ.set(1, 0, sin(rot.z));
	matrixZ.set(1, 1, cos(rot.z));
	matrixZ.set(2, 2, 1.0f);
	matrixZ.set(3, 3, 1.0f);

	return matrixZ * matrixY * matrixX;
}

Matrix4x4 Matrix4x4::Scale(const Vector3& scale)
{
	Matrix4x4 result = Matrix4x4();
	result.set(0, 0, scale.x);
	result.set(1, 1, scale.y);
	result.set(2, 2, scale.z);
	result.set(3, 3, 1.0f);
	return result;
}
