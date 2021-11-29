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

    // Camera's main buffer to write to (default to HDR + depth buffer).
    m_BufferTex = new BufferTexture(1600, 900, /*depth=*/ 32, TextureFormat::RGBAHalf);
    m_ImageEffectChain = new ImageEffectChain(this);
}

Camera::~Camera()
{
    delete m_Transform;
    delete m_BufferTex;
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

void Camera::draw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_BufferTex->id());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render scene

    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
    m_ImageEffectChain->render(m_BufferTex);
}

void Camera::addImageEffect(ImageEffect* effect)
{
    m_ImageEffectChain->add(effect);
}

void Camera::addBuffersToDebugWindow(DebugTextureListWindow& window) const
{
    window.add(m_BufferTex->depthTexture(), "Depth", /*flip=*/ true);
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
