#include "Camera.h"

Camera::Camera(const Vector3& pos, const Vector3& rot)
{
    m_Transform = new Transform(pos, rot, Vector3::one);
}

Camera::~Camera()
{
    delete m_Transform;
}

void Camera::update()
{
    m_ViewMatrix = Matrix4x4::View(
        m_Transform->getPosition(),
        m_Transform->getRotation()
    );
}

Transform* Camera::getTransform() const
{
    return m_Transform;
}

Matrix4x4 Camera::getViewMatrix() const
{
    return m_ViewMatrix;
}
