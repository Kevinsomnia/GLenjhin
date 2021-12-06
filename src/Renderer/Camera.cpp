#include "Camera.h"

Camera::Projection::Projection(float nearClip, float farClip)
    : nearClip(nearClip), farClip(farClip) { }
Camera::Projection::~Projection() { /* no-op. just here to make dynamic_cast work. */ }
Camera::PerspectiveProjection::PerspectiveProjection(float nearClip, float farClip, float fieldOfView)
    : Projection(nearClip, farClip), fieldOfView(fieldOfView) { }
Camera::OrthographicProjection::OrthographicProjection(float nearClip, float farClip, float size)
    : Projection(nearClip, farClip), size(size) { }

Camera::Camera(uint32_t pixelWidth, uint32_t pixelHeight, const Vector3& pos, const Vector3& rot, Projection& projection, CameraBufferFlags bufferFlags, bool deferred)
    : m_NearClip(projection.nearClip), m_FarClip(projection.farClip), m_FieldOfView(60.0f), m_OrthoSize(1.0f)
{
    assert(bufferFlags != CameraBufferFlags::None);

    float aspect = pixelWidth / static_cast<float>(pixelHeight);
    uint32_t depthBits = (bufferFlags & CameraBufferFlags::Depth) != CameraBufferFlags::None ? 32 : 0;

    m_Transform = new Transform(pos, rot, Vector3::one);

    // This needs to update whenever near or far clip plane changes.
    m_ProjectionParams = Vector4(
        m_NearClip,
        m_FarClip,
        m_FarClip - m_NearClip,
        2.0f * m_NearClip * m_FarClip
    );

    PerspectiveProjection* perspective = dynamic_cast<PerspectiveProjection*>(&projection);

    if (perspective)
    {
        m_FieldOfView = perspective->fieldOfView;
        m_ProjMatrix = Matrix4x4::Perspective(m_FieldOfView, aspect, m_NearClip, m_FarClip);
    }
    else
    {
        OrthographicProjection* orthographic = static_cast<OrthographicProjection*>(&projection);
        m_OrthoSize = orthographic->size;
        m_ProjMatrix = Matrix4x4::Orthographic(m_OrthoSize, aspect, m_NearClip, m_FarClip);
    }

    if (deferred)
    {
        m_GBuffers = new GeometryBuffers(pixelWidth, pixelHeight, depthBits);
        m_DeferredGeometryMat = new Material(new Shader("res\\shaders\\Deferred\\GeometryBuffers.glsl"));
        m_DeferredLightingMat = new Material(new Shader("res\\shaders\\Deferred\\DeferredLighting.glsl"));
        m_GBuffers->setGBufferTextures(*m_DeferredLightingMat);
    }
    else
    {
        m_GBuffers = nullptr;
    }

    TextureFormat colorFormat = (bufferFlags & CameraBufferFlags::Color) != CameraBufferFlags::None ? TextureFormat::RGBAHalf : TextureFormat::None;
    m_RenderTargetBuffer = new BufferTexture(pixelWidth, pixelHeight, depthBits, colorFormat);
    m_ImageEffectChain = new ImageEffectChain(this);
    m_BlitMat = new Material(new Shader("res\\shaders\\PostProcessing\\Common\\Copy.glsl"));
    m_FullscreenTriangle = new FullscreenTriangle(m_BlitMat);
    update();
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
    m_ViewProjMatrix = m_ProjMatrix * Matrix4x4::View(m_Transform->getPosition(), m_Transform->getRotation());
}

void Camera::draw(Scene* scene, bool drawSkybox)
{
    glViewport(0, 0, m_RenderTargetBuffer->width(), m_RenderTargetBuffer->height());

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

        if (scene)
        {
            // NOTE: multiple lights of same type are not supported yet!
            for (Light* light : scene->lights())
                light->setUniforms(*m_DeferredLightingMat);
        }

        m_DeferredLightingMat->setColor("u_AmbientColor", Color::FromBytes(50, 77, 89));
        m_DeferredLightingMat->setVector3("u_CameraPos", m_Transform->getPosition());
        m_FullscreenTriangle->setMaterial(m_DeferredLightingMat);
        m_FullscreenTriangle->draw();

        // Blit GBuffer depth to buffer texture depth so that forward-rendered objects (e.g. skybox, transparent objects) display properly.
        // It is important for both depth buffers to be in the same format.
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffers->id());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RenderTargetBuffer->id());
        glBlitFramebuffer(0, 0, m_GBuffers->width(), m_GBuffers->height(), 0, 0, m_RenderTargetBuffer->width(), m_RenderTargetBuffer->height(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);

        if (scene && drawSkybox)
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

            if (drawSkybox)
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
        window.add(m_GBuffers->normalSmoothGBuffer(), "GBuffer [RGBAFloat]: World Normal (RGB) Smoothness (A)", /*flip=*/ true);
        window.add(m_GBuffers->albedoMetalGBuffer(), "GBuffer [RGBA32]: Albedo (RGB) Metallic (A)", /*flip=*/ true);
        window.add(m_GBuffers->emissionGBuffer(), "GBuffer [RGBAHalf]: Emission (RGB)", /*flip=*/ true);
    }

    window.add(getDepthTexture(), "Camera Depth (R) [Float]", /*flip=*/ true);
}
