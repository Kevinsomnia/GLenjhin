#ifndef VECTOR_H
#define VECTOR_H

struct Vec3
{
	Vec3(float x, float y, float z)
		: x(x), y(y), z(z) {}

	float x;
	float y;
	float z;
};

#endif // VECTOR_H
