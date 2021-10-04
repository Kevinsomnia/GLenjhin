#include "Transform.h"

Transform::Transform()
	: m_Position(Vector3::zero), m_Rotation(Vector3::zero), m_Scale(Vector3::one), m_DirtyTRS(true)
{
}

Transform::Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale)
	: m_Position(position), m_Rotation(rotation), m_Scale(scale), m_DirtyTRS(true)
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

Matrix4x4 Transform::getTRS()
{
	if (m_DirtyTRS)
	{
		m_DirtyTRS = false;
		m_TRS = Matrix4x4::TRS(getPosition(), getRotation(), getScale());
	}

	return m_TRS;
}

void Transform::setPosition(const Vector3& position)
{
	m_Position = position;
	m_DirtyTRS = true;
}

void Transform::setRotation(const Vector3& rotation)
{
	m_Rotation = rotation;
	m_DirtyTRS = true;
}

void Transform::setScale(const Vector3& scale)
{
	m_Scale = scale;
	m_DirtyTRS = true;
}
