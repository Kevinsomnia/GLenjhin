#include "Camera.h"

Camera::Camera(const Vector3& pos, const Vector3& rot, float fieldOfView, float nearClip, float farClip, bool deferred)
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

    const uint32_t SCR_WIDTH = 1600;
    const uint32_t SCR_HEIGHT = 900;
    const uint32_t DEPTH = 32;

    if (deferred)
    {
        m_GBuffers = new GeometryBuffers(SCR_WIDTH, SCR_HEIGHT, DEPTH);
        m_DeferredGeometryMat = new Material(new Shader("res\\shaders\\Deferred\\GeometryBuffers.glsl"));
        m_DeferredLightingMat = new Material(new Shader("res\\shaders\\Deferred\\DeferredLighting.glsl"));
        m_GBuffers->setGBufferTextures(*m_DeferredLightingMat);
    }
    else
    {
        m_GBuffers = nullptr;
    }

    m_RenderTargetBuffer = new BufferTexture(SCR_WIDTH, SCR_HEIGHT, DEPTH, TextureFormat::RGBAHalf);
    m_ImageEffectChain = new ImageEffectChain(this);
    m_BlitMat = new Material(new Shader("res\\shaders\\PostProcessing\\Common\\Copy.glsl"));
    m_FullscreenTriangle = new FullscreenTriangle(m_BlitMat);
}

Camera::~Camera()
{
    delete m_Transform;
    delete m_RenderTargetBuffer;
    delete m_BlitMat;
    delete m_FullscreenTriangle;

    if (m_GBuffers)
    {
        delete m_GBuffers;
        delete m_DeferredGeometryMat;
        delete m_DeferredLightingMat;
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

void Camera::draw(Scene* scene)
{
    if (m_GBuffers)
    {
        // === Deferred ===
        // First pass: render scene to GBuffers + depth texture
        glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffers->id());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (scene)
            scene->drawGeometryPass(*this, *m_DeferredGeometryMat);

        // Second pass: calculate lighting in screen-space, output to color buffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_RenderTargetBuffer->id());
        glClear(GL_COLOR_BUFFER_BIT);

        if (scene)
        {
            // NOTE: multiple lights of same type are not supported!
            for (Light* light : scene->lights())
                light->bind(*m_DeferredLightingMat);
        }

        m_DeferredLightingMat->setVector3("u_CameraPos", m_Transform->getPosition());
        m_DeferredLightingMat->bind();
        m_FullscreenTriangle->setMaterial(m_DeferredLightingMat);
        m_FullscreenTriangle->draw();

        // Blit GBuffer depth to buffer texture depth so that forward-rendered objects (e.g. skybox, transparent objects) display properly.
        // It is important for both depth buffers to be in the same format.
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffers->id());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RenderTargetBuffer->id());
        glBlitFramebuffer(0, 0, m_GBuffers->width(), m_GBuffers->height(), 0, 0, m_RenderTargetBuffer->width(), m_RenderTargetBuffer->height(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);

        // Render skybox
        if (scene)
            scene->drawSkybox(*this);
    }
    else
    {
        // === Forward ===
        // Store scene color in buffer texture
        glBindFramebuffer(GL_FRAMEBUFFER, m_RenderTargetBuffer->id());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (scene)
        {
            Camera& cam = *this;
            scene->drawSkybox(cam);
            scene->drawEntities(cam);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, NULL);

    // Post processing
    m_ImageEffectChain->render(m_RenderTargetBuffer);
}

void Camera::blitToScreen() const
{
    const int SCREEN_WIDTH = 1600;
    const int SCREEN_HEIGHT = 900;

    // Render our buffer texture's color to the screen/default FBO.
    // This is not done automatically since sometimes we don't want the buffer to display on the screen.
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
    m_BlitMat->setTexture("u_MainTex", m_RenderTargetBuffer->colorTexture());
    m_FullscreenTriangle->setMaterial(m_BlitMat);
    m_FullscreenTriangle->draw();
}

void Camera::addImageEffect(ImageEffect* effect)
{
    m_ImageEffectChain->add(effect);
}

void Camera::addBuffersToDebugWindow(DebugTextureListWindow& window) const
{
    if (m_GBuffers)
    {
        window.add(m_GBuffers->positionGBuffer(), "GBuffer [RGBAFloat]: World Position (RGB)", /*flip=*/ true);
        window.add(m_GBuffers->normalSmoothGBuffer(), "GBuffer [RGBAHalf]: World Normal (RGB) Smoothness (A)", /*flip=*/ true);
        window.add(m_GBuffers->albedoMetalGBuffer(), "GBuffer [RGBA32]: Albedo (RGB) Metallic (A)", /*flip=*/ true);
        window.add(m_GBuffers->emissionGBuffer(), "GBuffer [RGBAHalf]: Emission (RGB)", /*flip=*/ true);
        window.add(m_GBuffers->depthTexture(), "Depth (R) [Float]", /*flip=*/ true);
    }
}
