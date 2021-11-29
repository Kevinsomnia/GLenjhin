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

    const int SCR_WIDTH = 1600;
    const int SCR_HEIGHT = 900;

    // Deferred
    m_GBuffers = new GeometryBuffers(SCR_WIDTH, SCR_HEIGHT);
    m_DeferredGeometryMat = new Material(new Shader("res\\shaders\\Deferred\\GeometryBuffers.shader"));

    m_ColorBuffer = new BufferTexture(SCR_WIDTH, SCR_HEIGHT, /*depth=*/ 32, TextureFormat::RGBAHalf);
    m_ImageEffectChain = new ImageEffectChain(this);
    m_BlitMat = new Material(new Shader("res\\shaders\\PostProcessing\\Common\\Copy.shader"));
    m_BlitTriangle = new FullscreenTriangle(m_BlitMat);
}

Camera::~Camera()
{
    delete m_Transform;
    delete m_ColorBuffer;
    delete m_BlitMat;
    delete m_BlitTriangle;

    if (m_GBuffers)
    {
        delete m_GBuffers;
        delete m_DeferredGeometryMat;
    }
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
    if (m_GBuffers)
    {
        // First pass: render scene to GBuffers + depth texture
        glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffers->id());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (scene)
            scene->drawGeometryPass(*this, *m_DeferredGeometryMat);
    }

    // Store scene color in buffer tex
    glBindFramebuffer(GL_FRAMEBUFFER, m_ColorBuffer->id());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (scene)
        scene->draw(*this, /*drawSkybox=*/ true);

    glBindFramebuffer(GL_FRAMEBUFFER, NULL);

    // Post processing
    m_ImageEffectChain->render(m_ColorBuffer);
}

void Camera::blitToScreen() const
{
    // Render our buffer texture's color to the screen/default FBO.
    // This is not done automatically since sometimes we don't want the buffer to display on the screen.
    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
    m_BlitMat->setTexture("u_MainTex", m_ColorBuffer->colorTexture());
    m_BlitTriangle->draw();
}

void Camera::addImageEffect(ImageEffect* effect)
{
    m_ImageEffectChain->add(effect);
}

void Camera::addBuffersToDebugWindow(DebugTextureListWindow& window) const
{
    window.add(m_GBuffers->positionGBuffer(), "GBuffer: World Position (RGB)", /*flip=*/ true);
    window.add(m_GBuffers->normalGBuffer(), "GBuffer: World Normal (RGB)", /*flip=*/ true);
    window.add(m_GBuffers->albedoSpecGBuffer(), "GBuffer: Albedo (RGB) Specular (A)", /*flip=*/ true);
    window.add(m_GBuffers->depthTexture(), "Depth", /*flip=*/ true);
}
