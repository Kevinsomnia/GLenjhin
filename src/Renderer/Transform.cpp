#include "Transform.h"

Transform::Transform()
    : m_Position(Vector3::zero), m_Rotation(Quaternion::identity), m_Scale(Vector3::one), m_DirtyTRS(true)
{
}

Transform::Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
    : m_Position(position), m_Rotation(rotation), m_Scale(scale), m_DirtyTRS(true)
{
}

Vector3 Transform::getPosition() const
{
    return m_Position;
}

Quaternion Transform::getRotation() const
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

Vector3 Transform::getForward() const
{
    return transformDirection(Vector3::forward);
}

Vector3 Transform::transformDirection(const Vector3& dir) const
{
    return m_TRS.multiplyVector(dir);
}

void Transform::setPosition(const Vector3& position)
{
    m_Position = position;
    m_DirtyTRS = true;
}

void Transform::setRotation(const Quaternion& rotation)
{
    m_Rotation = rotation;
    m_DirtyTRS = true;
}

void Transform::setScale(const Vector3& scale)
{
    m_Scale = scale;
    m_DirtyTRS = true;
}

void Transform::translate(const Vector3& v, Space space)
{
    if (space == Space::World)
    {
        m_Position += v;
    }
    else
    {
        m_Position += getTRS().multiplyVector(v);
    }

    m_DirtyTRS = true;
}

void Transform::rotate(const Vector3& radians, Space space)
{
    Vector3 eulers = (space == Space::World) ? radians : getTRS().multiplyVector(radians);
    m_Rotation *= Quaternion::EulerRadians(eulers);
    m_DirtyTRS = true;
}
