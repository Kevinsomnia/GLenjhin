#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <iostream>
#include "../Math/Vector.h"

using std::cout;
using std::cerr;
using std::endl;

class Transform
{
public:
	Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale);

	Vector3 getPosition() const;
	Vector3 getRotation() const;
	Vector3 getScale() const;
	void setPosition(const Vector3& position);
	void setRotation(const Vector3& rotation);
	void setScale(const Vector3& scale);
private:
	Vector3 m_Position;
	Vector3 m_Rotation;
	Vector3 m_Scale;
};

#endif // TRANSFORM_H
