#include "Camera.h"

// Projection constructors.
Camera::Projection::Projection(float nearClip, float farClip)
    : nearClip(nearClip), farClip(farClip) { }
Camera::Projection::~Projection() { /* no-op. just here to make dynamic_cast work. */ }
Camera::PerspectiveProjection::PerspectiveProjection(float nearClip, float farClip, float fieldOfView)
    : Projection(nearClip, farClip), fieldOfView(fieldOfView) { }
Camera::OrthographicProjection::OrthographicProjection(float nearClip, float farClip, float size)
    : Projection(nearClip, farClip), size(size) { }


Camera::Camera(uint32_t pixelWidth, uint32_t pixelHeight, const Vector3& pos, const Vector3& rot, Projection& projection, CameraBufferFlags bufferFlags, bool deferred)
    : m_NearClip(projection.nearClip), m_FarClip(projection.farClip), m_FieldOfView(60.0f), m_OrthoSize(1.0f), m_BufferFlags(bufferFlags)
{
    assert(bufferFlags != CameraBufferFlags::None);

    float aspect = pixelWidth / static_cast<float>(pixelHeight);
    TextureFormat depthFormat = (bufferFlags & CameraBufferFlags::Depth) != CameraBufferFlags::None ? TextureFormat::Depth32 : TextureFormat::None;

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
        m_ProjectionMatrix = Matrix4x4::Perspective(m_FieldOfView, aspect, m_NearClip, m_FarClip);
    }
    else
    {
        OrthographicProjection* orthographic = static_cast<OrthographicProjection*>(&projection);
        m_OrthoSize = orthographic->size;
        m_ProjectionMatrix = Matrix4x4::Orthographic(m_OrthoSize, aspect, m_NearClip, m_FarClip);
    }

    if (deferred)
    {
        bool renderToMotionVectors = (bufferFlags & CameraBufferFlags::MotionVectors) != CameraBufferFlags::None;
        m_GBuffers = new GeometryBuffers(pixelWidth, pixelHeight, depthFormat, renderToMotionVectors);
        m_DeferredGeometryMat = new Material(Shader::Load("res\\shaders\\Deferred\\GeometryBuffers.glsl"));
        m_DeferredLightingMat = new Material(Shader::Load("res\\shaders\\Deferred\\DeferredLighting.glsl"));
        m_GBuffers->setGBufferTextures(*m_DeferredLightingMat);

        if (renderToMotionVectors)
            m_BgMotionVectorsMat = new Material(Shader::Load("res\\shaders\\BackgroundMotionVectors.glsl"));

        m_DeferredChain = new DeferredEffectChain(this);
    }
    else
    {
        m_GBuffers = nullptr;
    }

    TextureFormat colorFormat = (bufferFlags & CameraBufferFlags::Color) != CameraBufferFlags::None ? TextureFormat::RGBAHalf : TextureFormat::None;
    m_RenderTargetBuffer = new BufferTexture(pixelWidth, pixelHeight, colorFormat, depthFormat);
    m_PostProcessChain = new PostProcessEffectChain(this);
    m_BlitMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\Common\\Copy.glsl"));

    update();
    m_PrevViewProjectionMatrix = m_ViewProjectionMatrix;
}

Camera::~Camera()
{
    delete m_Transform;
    delete m_RenderTargetBuffer;
    delete m_BlitMat;

    if (m_BgMotionVectorsMat)
        delete m_BgMotionVectorsMat;

    if (m_GBuffers)
    {
        delete m_GBuffers;
        delete m_DeferredGeometryMat;
        delete m_DeferredLightingMat;
    }
}

void Camera::update()
{
    m_ViewMatrix = Matrix4x4::View(m_Transform->position(), m_Transform->rotation());
    m_PrevViewProjectionMatrix = m_ViewProjectionMatrix;
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Camera::draw(Scene* scene, bool drawSkybox)
{
    if (m_GBuffers)
    {
        // === Deferred ===
        // First pass: render scene to GBuffers + depth texture
        m_GBuffers->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (scene)
            scene->drawGeometryPass(*this, *m_DeferredGeometryMat);

        // Render motion vectors for background pixels (geometry pass is the foreground).
        // Any motion beyond the far clip plane are considered infinitely far away, so we only camera rotation will apply motion.
        renderBackgroundMotionVectors();

        // Deferred image effects manipulate the GBuffers directly
        m_DeferredChain->render();

        // Second pass: calculate lighting in screen-space, output to color buffer
        m_RenderTargetBuffer->bind();

        if (scene)
        {
            // NOTE: multiple lights of same type are not supported yet!
            for (Light* light : scene->lights())
                light->setUniforms(*m_DeferredLightingMat);
        }

        m_DeferredLightingMat->setColor("u_AmbientColor", ColorByte(50, 81, 107));
        m_DeferredLightingMat->setVector3("u_CameraPos", m_Transform->position());
        FullscreenTriangle::Draw(*m_DeferredLightingMat, /*depthTest=*/ false);

        // Blit GBuffer depth to buffer texture depth so that forward-rendered objects (e.g. skybox, transparent objects) display properly.
        // It is important for both depth buffers to be in the same format.
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffers->id());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RenderTargetBuffer->id());
        GlobalStats::AddFramebufferBindCall();
        glBlitFramebuffer(0, 0, m_GBuffers->width(), m_GBuffers->height(), 0, 0, m_RenderTargetBuffer->width(), m_RenderTargetBuffer->height(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);

        if (scene && drawSkybox)
            scene->drawSkybox(*this);
    }
    else
    {
        // === Forward ===
        // Store scene color in buffer texture
        m_RenderTargetBuffer->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (scene)
        {
            Camera& cam = *this;

            if (drawSkybox)
                scene->drawSkybox(cam);

            scene->drawEntities(cam);
        }
    }

    m_PostProcessChain->render();
}

void Camera::blitToScreen() const
{
    const int SCREEN_WIDTH = 1600;
    const int SCREEN_HEIGHT = 900;

    // Render our buffer texture's color to the screen/default FBO.
    // This is not done automatically since sometimes we don't want the buffer to display on the screen.
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
    GlobalStats::AddFramebufferBindCall();
    m_BlitMat->setTexture("u_MainTex", m_RenderTargetBuffer->colorTexture());
    FullscreenTriangle::Draw(*m_BlitMat, /*depthTest=*/ false);
}

void Camera::addDeferredEffect(DeferredEffect* effect)
{
    if (m_DeferredChain)
        m_DeferredChain->add(effect);
}

void Camera::addPostProcessEffect(PostProcessEffect* effect)
{
    m_PostProcessChain->add(effect);
}

void Camera::addBuffersToDebugWindow(DebugWindow& window) const
{
    if (m_GBuffers)
    {
        window.addTexture(m_GBuffers->positionGBuffer(), "GBuffer [RGBAFloat]: World Position (RGB)", /*flip=*/ true, DebugWindow::ElementSizeMode::ConstrainToWindowWidth);
        window.addTexture(m_GBuffers->normalSmoothGBuffer(), "GBuffer [RGBAHalf]: World Normal (RGB) Smoothness (A)", /*flip=*/ true, DebugWindow::ElementSizeMode::ConstrainToWindowWidth);
        window.addTexture(m_GBuffers->albedoMetalGBuffer(), "GBuffer [RGBA8]: Albedo (RGB) Metallic (A)", /*flip=*/ true, DebugWindow::ElementSizeMode::ConstrainToWindowWidth);
        window.addTexture(m_GBuffers->emissionOcclGBuffer(), "GBuffer [RGBAHalf]: Emission (RGB) Occlusion (A)", /*flip=*/ true, DebugWindow::ElementSizeMode::ConstrainToWindowWidth);
        window.addTexture(m_GBuffers->motionVectorsTexture(), "Motion Vectors [RGHalf]: Screen-space Motion (RG)", /*flip=*/ true, DebugWindow::ElementSizeMode::ConstrainToWindowWidth);
    }

    window.addTexture(depthTexture(), "Camera Depth (R) [Float]", /*flip=*/ true, DebugWindow::ElementSizeMode::ConstrainToWindowWidth);
}

Vector3 Camera::worldToViewportPoint(const Vector3& pos) const
{
    Vector3 viewPos = m_ViewProjectionMatrix.multiplyPoint3x4(pos);
    // Perspective divide XY and normalize to [0, 1], but leave Z untouched so that it's in world units.
    float halfOverW = 0.5f / ((pos.x * m_ViewProjectionMatrix[3]) + (pos.y * m_ViewProjectionMatrix[7]) + (pos.z * m_ViewProjectionMatrix[11]) + m_ViewProjectionMatrix[15]);
    return Vector3((viewPos.x * halfOverW) + 0.5f, (viewPos.y * halfOverW) + 0.5f, viewPos.z + (m_NearClip * 2.0f));
}

void Camera::renderBackgroundMotionVectors()
{
    if (!motionVectorsTexture())
        return;

    m_BgMotionVectorsMat->setMatrix4x4("u_PrevVP", prevViewProjectionMatrix());
    m_BgMotionVectorsMat->setMatrix4x4("u_CurrVP", viewProjectionMatrix());
    FullscreenTriangle::Draw(*m_BgMotionVectorsMat, /*depthTest=*/ true);
}
