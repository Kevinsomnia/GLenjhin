#ifndef VECTOR_H
#define VECTOR_H

struct Vector2
{
	Vector2(float x, float y)
		: x(x), y(y) {}

	float x;
	float y;

	static float Dot(const Vector2& a, const Vector2& b);
};

struct Vector3
{
	Vector3(float x, float y, float z)
		: x(x), y(y), z(z) {}

	float x;
	float y;
	float z;

	static float Dot(const Vector3& a, const Vector3& b);
};

struct Vector4
{
	Vector4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w) {}

	float x;
	float y;
	float z;
	float w;

	static float Dot(const Vector4& a, const Vector4& b);
};

#endif // VECTOR_H
