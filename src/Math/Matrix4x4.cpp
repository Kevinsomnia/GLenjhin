#include "Matrix4x4.h"

inline uint32_t getIndex(int row, int column)
{
	return (column * 4) + row;
}

Matrix4x4::Matrix4x4()
{
	std::fill(std::begin(values), std::end(values), 0.0f);
}

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
	float cosX = cos(rot.x);
	float cosY = cos(rot.y);
	float cosZ = cos(rot.z);
	float sinX = sin(rot.x);
	float sinY = sin(rot.y);
	float sinZ = sin(rot.z);

	// Be sure to update TRS() if this ordering changes: ZYX
	Matrix4x4 result = Matrix4x4();
	result.set(0, 0, cosZ * cosY);
	result.set(0, 1, (-sinZ * cosX) + (cosZ * sinY * sinX));
	result.set(0, 2, (sinZ * sinX) + (cosZ * sinY * cosX));
	result.set(1, 0, sinZ * cosY);
	result.set(1, 1, (cosZ * cosX) + (sinZ * sinY * sinX));
	result.set(1, 2, (cosZ * -sinX) + (sinZ * sinY * cosX));
	result.set(2, 0, -sinY);
	result.set(2, 1, cosY * sinX);
	result.set(2, 2, cosY * cosX);
	result.set(3, 3, 1.0f);

	return result;
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

Matrix4x4 Matrix4x4::TRS(const Vector3& pos, const Vector3& rot, const Vector3& scale)
{
	float cosX = cos(rot.x);
	float cosY = cos(rot.y);
	float cosZ = cos(rot.z);
	float sinX = sin(rot.x);
	float sinY = sin(rot.y);
	float sinZ = sin(rot.z);

	Matrix4x4 trs = Matrix4x4();
	trs.set(0, 0, (cosZ * cosY) * scale.x);
	trs.set(0, 1, (-sinZ * cosX + cosZ * sinY * sinX) * scale.y);
	trs.set(0, 2, (sinZ * sinX + cosZ * sinY * cosX) * scale.z);
	trs.set(0, 3, pos.x);
	trs.set(1, 0, (sinZ * cosY) * scale.x);
	trs.set(1, 1, (cosZ * cosX + sinZ * sinY * sinX) * scale.y);
	trs.set(1, 2, (cosZ * -sinX + sinZ * sinY * cosX) * scale.z);
	trs.set(1, 3, pos.y);
	trs.set(2, 0, (-sinY) * scale.x);
	trs.set(2, 1, (cosY * sinX) * scale.y);
	trs.set(2, 2, (cosY * cosX) * scale.z);
	trs.set(2, 3, pos.z);
	trs.set(3, 3, 1.0f);

	return trs;
}
