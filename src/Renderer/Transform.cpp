#include "Transform.h"

Transform::Transform() : Transform(Vector3::zero, Vector3::zero, Vector3::zero) { }
Transform::Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale)
    : m_Position(position), m_Rotation(rotation), m_Scale(scale), m_DirtyTRS(true)
{
    m_PrevTRS = getTRS();
}

Matrix4x4 Transform::getTRS()
{
    if (m_DirtyTRS)
    {
        m_DirtyTRS = false;
        m_TRS = Matrix4x4::TRS(m_Position, m_Rotation, m_Scale);
    }

    return m_TRS;
}

Vector3 Transform::getForward()
{
    return transformDirection(Vector3::forward);
}

Vector3 Transform::transformDirection(const Vector3& dir)
{
    return getTRS().multiplyVector(dir);
}

void Transform::earlyUpdate()
{
    m_PrevTRS = getTRS();
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

void Transform::rotate(const Vector3& r, Space space)
{
    if (space == Space::World)
    {
        m_Rotation += r;
    }
    else
    {
        m_Rotation += getTRS().multiplyVector(r);
    }

    m_DirtyTRS = true;
}
