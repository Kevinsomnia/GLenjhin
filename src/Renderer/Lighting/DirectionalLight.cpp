#include "Lighting.h"

DirectionalLight::DirectionalLight(const Vector3& pos, const Vector3& rot)
    : Light(pos, rot)
{
    auto projection = Camera::OrthographicProjection(/*nearClip=*/ -200.0f, /*farClip=*/ 200.0f, /*size=*/ 14.0f);
    m_DepthCamera = new Camera(/*pixelWidth=*/ 2048, /*pixelHeight=*/ 2048, pos, rot, projection, CameraBufferFlags::Depth, /*deferred=*/ false);

    // Setup depth map to clamp to a border color so that samples outside the shadowmap will properly be "unshadowed"
    uint32_t depthTexID = m_DepthCamera->getDepthTexture()->id();
    glTextureParameteri(depthTexID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(depthTexID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTextureParameterfv(depthTexID, GL_TEXTURE_BORDER_COLOR, Color::White());
}

DirectionalLight::~DirectionalLight()
{
    if (m_DepthCamera)
        delete m_DepthCamera;
}

void DirectionalLight::bind(Material& mat) const
{
    Vector3 fwd = m_Transform->getTRS().multiplyVector(Vector3::forward);
    Texture2D* shadowMap = m_DepthCamera ? m_DepthCamera->getDepthTexture() : nullptr;
    mat.setVector3("u_DirLightDir", fwd);

    if (m_DepthCamera)
    {
        Texture2D* shadowMap = m_DepthCamera->getDepthTexture();
        mat.setTexture("u_DirShadows", shadowMap);
        mat.setVector2("u_DirShadowsTexelSize", shadowMap->texelSize());
        mat.setMatrix("u_DirLightMatrix", m_DepthCamera->getViewProjMatrix());
        mat.setColor("u_DirLightColor", Color(1.0f, 0.91f, 0.8f) * 2.0f);
    }
    else
    {
        mat.setTexture("u_DirShadows", nullptr);
    }
}

void DirectionalLight::update()
{
    if (m_DepthCamera)
        m_DepthCamera->update();
}

void DirectionalLight::renderShadows(const Scene* scene) const
{
    if (!m_DepthCamera || !scene)
        return;

    BufferTexture* bufferTex = m_DepthCamera->getRenderTargetBuffer();
    glViewport(0, 0, bufferTex->width(), bufferTex->height());
    glBindFramebuffer(GL_FRAMEBUFFER, bufferTex->id());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->drawShadowPass(*this, *m_ShadowMat);
}

Matrix4x4 DirectionalLight::getLightMatrix() const
{
    if (m_DepthCamera)
        return m_DepthCamera->getViewProjMatrix();

    return Matrix4x4::identity;
}
