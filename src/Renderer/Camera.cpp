#include "Camera.h"

Camera::Camera(const Vector3& pos, const Vector3& rot, float fieldOfView, float nearClip, float farClip)
    : m_FieldOfView(fieldOfView), m_NearClip(nearClip), m_FarClip(farClip)
{
    m_Transform = new Transform(pos, rot, Vector3::one);
    update();

    // This needs to update whenever near or far clip plane changes.
    m_ProjectionParams = Vector4(
        m_NearClip,
        m_FarClip,
        m_FarClip - m_NearClip,
        2.0f * m_NearClip * m_FarClip
    );
}

Camera::~Camera()
{
    delete m_Transform;
}

void Camera::update()
{
    m_ViewProjMatrix = 
        Matrix4x4::Perspective(m_FieldOfView, 16.0f / 9.0f, m_NearClip, m_FarClip) *
        Matrix4x4::View(
            m_Transform->getPosition(),
            m_Transform->getRotation()
        );
}

Transform* Camera::getTransform() const
{
    return m_Transform;
}

Matrix4x4 Camera::getViewProjMatrix() const
{
    return m_ViewProjMatrix;
}

Vector4 Camera::getProjectionParams() const
{
    return m_ProjectionParams;
}
