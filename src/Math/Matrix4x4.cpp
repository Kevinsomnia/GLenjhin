#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
	memset(values, 0, sizeof(values));
}

Matrix4x4::Matrix4x4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3)
{
	setRow(0, row0);
	setRow(1, row1);
	setRow(2, row2);
	setRow(3, row3);
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

	return Matrix4x4(
		Vector4(
			Vector4::Dot(a0, b0),
			Vector4::Dot(a0, b1),
			Vector4::Dot(a0, b2),
			Vector4::Dot(a0, b3)
		),
		Vector4(
			Vector4::Dot(a1, b0),
			Vector4::Dot(a1, b1),
			Vector4::Dot(a1, b2),
			Vector4::Dot(a1, b3)
		),
		Vector4(
			Vector4::Dot(a2, b0),
			Vector4::Dot(a2, b1),
			Vector4::Dot(a2, b2),
			Vector4::Dot(a2, b3)
		),
		Vector4(
			Vector4::Dot(a3, b0),
			Vector4::Dot(a3, b1),
			Vector4::Dot(a3, b2),
			Vector4::Dot(a3, b3)
		)
	);
}

Matrix4x4 Matrix4x4::identity = Matrix4x4(
	Vector4(1.0f, 0.0f, 0.0f, 0.0f),
	Vector4(0.0f, 1.0f, 0.0f, 0.0f),
	Vector4(0.0f, 0.0f, 1.0f, 0.0f),
	Vector4(0.0f, 0.0f, 0.0f, 1.0f)
);

Matrix4x4 Matrix4x4::Translate(const Vector3& pos)
{
	return Matrix4x4(
		Vector4(1.0f, 0.0f, 0.0f, pos.getX()),
		Vector4(0.0f, 1.0f, 0.0f, pos.getY()),
		Vector4(0.0f, 0.0f, 1.0f, pos.getZ()),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

Matrix4x4 Matrix4x4::Rotate(const Vector3& rot)
{
	float cosX = cos(rot.getX());
	float cosY = cos(rot.getY());
	float cosZ = cos(rot.getZ());
	float sinX = sin(rot.getX());
	float sinY = sin(rot.getY());
	float sinZ = sin(rot.getZ());

	// Be sure to update TRS() too if this ordering changes: ZYX
	return Matrix4x4(
		Vector4(
			cosZ * cosY,
			-sinZ * cosX + cosZ * sinY * sinX,
			sinZ * sinX + cosZ * sinY * cosX,
			0.0f
		),
		Vector4(
			sinZ * cosY,
			cosZ * cosX + sinZ * sinY * sinX,
			cosZ * -sinX + sinZ * sinY * cosX,
			0.0f
		),
		Vector4(
			-sinY,
			cosY * sinX,
			cosY * cosX,
			0.0f
		),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

Matrix4x4 Matrix4x4::Scale(const Vector3& scale)
{
	return Matrix4x4(
		Vector4(scale.getX(), 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, scale.getY(), 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, scale.getZ(), 0.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

Matrix4x4 Matrix4x4::TRS(const Vector3& pos, const Vector3& rot, const Vector3& scale)
{
	float cosX = cos(rot.getX());
	float cosY = cos(rot.getY());
	float cosZ = cos(rot.getZ());
	float sinX = sin(rot.getX());
	float sinY = sin(rot.getY());
	float sinZ = sin(rot.getZ());

	return Matrix4x4(
		Vector4(
			(cosZ * cosY) * scale.getX(),
			(-sinZ * cosX + cosZ * sinY * sinX) * scale.getY(),
			(sinZ * sinX + cosZ * sinY * cosX) * scale.getZ(),
			pos.getX()
		),
		Vector4(
			(sinZ * cosY) * scale.getX(),
			(cosZ * cosX + sinZ * sinY * sinX) * scale.getY(),
			(cosZ * -sinX + sinZ * sinY * cosX) * scale.getZ(),
			pos.getY()
		),
		Vector4(
			(-sinY) * scale.getX(),
			(cosY * sinX) * scale.getY(),
			(cosY * cosX) * scale.getZ(),
			pos.getZ()
		),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

Matrix4x4 Matrix4x4::View(const Vector3& pos, const Vector3& rot)
{
	Matrix4x4 rotMatrix = Matrix4x4::Rotate(rot);

	// Calculate "look at" vector (fwd) by using the camera transform rotation.
	Vector3 fwd = rotMatrix.multiplyVector(Vector3::forward);
	Vector3 up = rotMatrix.multiplyVector(Vector3::up);
	Vector3 right = Vector3::Cross(up, fwd);

	return Matrix4x4(
		Vector4(right.getX(), right.getY(), right.getZ(), -Vector3::Dot(right, pos)),
		Vector4(up.getX(), up.getY(), up.getZ(), -Vector3::Dot(up, pos)),
		Vector4(-fwd.getX(), -fwd.getY(), -fwd.getZ(), Vector3::Dot(fwd, pos)),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

Matrix4x4 Matrix4x4::Orthographic(float size, float aspect, float near, float far)
{
	return Matrix4x4(
		Vector4(1.0f / (size * aspect), 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, 1.0f / size, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, -2.0f / (far - near), -(far + near) / (far - near)),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

Matrix4x4 Matrix4x4::Perspective(float fov, float aspect, float near, float far)
{
	float tanHalfFov = tan(degToRad(fov * 0.5f));

	return Matrix4x4(
		Vector4(1.0f / (tanHalfFov * aspect), 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, 1.0f / tanHalfFov, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, -(far + near) / (far - near), -(2.0f * far * near) / (far - near)),
		Vector4(0.0f, 0.0f, -1.0f, 0.0f)
	);
}
