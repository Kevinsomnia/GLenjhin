#include "Transform.h"

Transform::Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale)
	: m_Position(position), m_Rotation(rotation), m_Scale(scale)
{
}

Vector3 Transform::getPosition() const
{
	return m_Position;
}

Vector3 Transform::getRotation() const
{
	return m_Rotation;
}

Vector3 Transform::getScale() const
{
	return m_Scale;
}

void Transform::setPosition(const Vector3& position)
{
	m_Position = position;
}

void Transform::setRotation(const Vector3& rotation)
{
	m_Rotation = rotation;
}

void Transform::setScale(const Vector3& scale)
{
	m_Scale = scale;
}
