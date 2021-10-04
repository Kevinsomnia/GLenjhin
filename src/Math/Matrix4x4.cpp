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
		Vector4(1.0f, 0.0f, 0.0f, pos.x),
		Vector4(0.0f, 1.0f, 0.0f, pos.y),
		Vector4(0.0f, 0.0f, 1.0f, pos.z),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

Matrix4x4 Matrix4x4::Rotate(const Vector3& rot)
{
	float cosX = cos(rot.x);
	float cosY = cos(rot.y);
	float cosZ = cos(rot.z);
	float sinX = sin(rot.x);
	float sinY = sin(rot.y);
	float sinZ = sin(rot.z);

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
		Vector4(scale.x, 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, scale.y, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, scale.z, 0.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

Matrix4x4 Matrix4x4::TRS(const Vector3& pos, const Vector3& rot, const Vector3& scale)
{
	float cosX = cos(rot.x);
	float cosY = cos(rot.y);
	float cosZ = cos(rot.z);
	float sinX = sin(rot.x);
	float sinY = sin(rot.y);
	float sinZ = sin(rot.z);

	return Matrix4x4(
		Vector4(
			(cosZ * cosY) * scale.x,
			(-sinZ * cosX + cosZ * sinY * sinX) * scale.y,
			(sinZ * sinX + cosZ * sinY * cosX) * scale.z,
			pos.x
		),
		Vector4(
			(sinZ * cosY) * scale.x,
			(cosZ * cosX + sinZ * sinY * sinX) * scale.y,
			(cosZ * -sinX + sinZ * sinY * cosX) * scale.z,
			pos.y
		),
		Vector4(
			(-sinY) * scale.x,
			(cosY * sinX) * scale.y,
			(cosY * cosX) * scale.z,
			pos.z
		),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

Matrix4x4 Matrix4x4::View(const Vector3& pos, const Vector3& rot)
{
	Matrix4x4 rotMatrix = Matrix4x4::Rotate(rot);

	// Calculate "look at" vector (fwd) by using the camera transform rotation. The look at vector is (pos - targetPoint), aka the
	// targetPoint's Z will be facing the camera's position (pos). Since we want the targetPoint to *always* be in front of the camera
	// in local space for the provided rotation to work, the local space "look at" direction will always be (0, 0, -1).
	// We simply use the rotMatrix (camera's world space rotation matrix) to transform this local vector into world space.
	Vector3 fwd = rotMatrix.multiplyVector(Vector3::back);
	Vector3 up = rotMatrix.multiplyVector(Vector3::up);
	Vector3 right = Vector3::Cross(up, fwd);

	return Matrix4x4(
		Vector4(right.x, up.x, fwd.x, -right.x * pos.x - up.x * pos.y - fwd.x * pos.z),
		Vector4(right.y, up.y, fwd.y, -right.y * pos.x - up.y * pos.y - fwd.y * pos.z),
		Vector4(right.z, up.z, fwd.z, -right.z * pos.x - up.z * pos.y - fwd.z * pos.z),
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
