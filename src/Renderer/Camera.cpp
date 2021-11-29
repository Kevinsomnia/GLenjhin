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
    m_BlitMat = new Material(new Shader("res\\shaders\\PostProcessing\\Common\\Copy.shader"));
    m_BlitTriangle = new FullscreenTriangle(m_BlitMat);
}

Camera::~Camera()
{
    delete m_Transform;
    delete m_BufferTex;
    delete m_BlitMat;
    delete m_BlitTriangle;
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

void Camera::draw(const Scene* scene)
{
    // Store scene color in buffer tex
    glBindFramebuffer(GL_FRAMEBUFFER, m_BufferTex->id());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (scene)
        scene->draw(*this, /*drawSkybox=*/ true);

    glBindFramebuffer(GL_FRAMEBUFFER, NULL);

    // Post processing
    m_ImageEffectChain->render(m_BufferTex);
}

void Camera::blitToScreen() const
{
    // Render our buffer texture's color to the screen/default FBO.
    // This is not done automatically since sometimes we don't want the buffer to display on the screen.
    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
    m_BlitMat->setTexture("u_MainTex", m_BufferTex->colorTexture());
    m_BlitTriangle->draw();
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
