#include "Vector.h"

// ====================
// Vector2
// ====================

Vector2 Vector2::zero = Vector2();
Vector2 Vector2::left = Vector2(-1.0f, 0.0f);
Vector2 Vector2::right = Vector2(1.0f, 0.0f);
Vector2 Vector2::down = Vector2(0.0f, -1.0f);
Vector2 Vector2::up = Vector2(0.0f, 1.0f);

float Vector2::Dot(const Vector2& a, const Vector2& b)
{
	return a.x * b.x + a.y * b.y;
}

float Vector2::Cross(const Vector2& a, const Vector2& b)
{
	// Returns the area of the parallelogram formed by a and b.
	return a.x * b.y - a.y * b.x;
}

// ====================
// Vector3
// ====================

Vector3 Vector3::zero = Vector3();
Vector3 Vector3::left = Vector3(-1.0f, 0.0f, 0.0f);
Vector3 Vector3::right = Vector3(1.0f, 0.0f, 0.0f);
Vector3 Vector3::back = Vector3(0.0f, 0.0f, -1.0f);
Vector3 Vector3::forward = Vector3(0.0f, 0.0f, 1.0f);
Vector3 Vector3::down = Vector3(0.0f, -1.0f, 0.0f);
Vector3 Vector3::up = Vector3(0.0f, 1.0f, 0.0f);

float Vector3::Dot(const Vector3& a, const Vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
{
	return Vector3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

// ====================
// Vector4
// ====================

Vector4 Vector4::zero = Vector4();

float Vector4::Dot(const Vector4& a, const Vector4& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
