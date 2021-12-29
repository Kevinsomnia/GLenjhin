#include "Transform.h"

Transform::Transform() : Transform(Vector3::zero, Vector3::zero, Vector3::zero) { }
Transform::Transform(const Vector3& localPosition, const Vector3& localRotation, const Vector3& localScale)
    : m_LocalPosition(localPosition), m_LocalRotation(localRotation), m_LocalScale(localScale), m_DirtyTRS(true)
{
    m_PrevWorldTRS = worldTRS();
}

Matrix4x4 Transform::worldTRS()
{
    if (m_DirtyTRS)
    {
        m_DirtyTRS = false;
        m_WorldTRS = Matrix4x4::TRS(m_LocalPosition, m_LocalRotation, m_LocalScale);
    }

    return m_WorldTRS;
}

Vector3 Transform::forward()
{
    return transformDirection(Vector3::forward);
}

Vector3 Transform::transformDirection(const Vector3& dir)
{
    return worldTRS().multiplyVector(dir);
}

void Transform::earlyUpdate()
{
    m_PrevWorldTRS = worldTRS();
}

void Transform::setLocalPosition(const Vector3& position)
{
    m_LocalPosition = position;
    m_DirtyTRS = true;
}

void Transform::setLocalRotation(const Vector3& rotation)
{
    m_LocalRotation = rotation;
    m_DirtyTRS = true;
}

void Transform::setLocalScale(const Vector3& scale)
{
    m_LocalScale = scale;
    m_DirtyTRS = true;
}

void Transform::translate(const Vector3& v, Space space)
{
    if (space == Space::World)
    {
        // TODO: final world rotation should affect this vector. (inverse TRS of parent upwards?)
        m_LocalPosition += v;
    }
    else
    {
        // v is already defined in local space
        m_LocalPosition += worldTRS().multiplyVector(v);
    }

    m_DirtyTRS = true;
}

void Transform::rotate(const Vector3& r, Space space)
{
    if (space == Space::World)
    {
        // TODO: final world rotation should affect this vector. (inverse TRS of parent upwards?)
        m_LocalRotation += r;
    }
    else
    {
        m_LocalRotation += worldTRS().multiplyVector(r);
    }

    m_DirtyTRS = true;
}
