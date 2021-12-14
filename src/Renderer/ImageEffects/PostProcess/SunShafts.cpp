#include "SunShafts.h"

SunShafts::SunShafts() : PostProcessEffect("res\\shaders\\ImageEffects\\SunShafts\\Combine.glsl")
{
    m_DepthFilterMat = new Material(new Shader("res\\shaders\\ImageEffects\\SunShafts\\DepthFilter.glsl"));
    m_RadialBlurMat = new Material(new Shader("res\\shaders\\ImageEffects\\SunShafts\\RadialBlur.glsl"));
}

SunShafts::~SunShafts()
{
    delete m_DepthFilterMat;
    delete m_RadialBlurMat;

    if (m_Initialized)
    {
        for (size_t i = 0; i < m_Buffers.size(); i++)
            delete m_Buffers[i];
    }
}

void SunShafts::lazyInitialize(Camera* camera)
{
    if (m_Initialized)
        return;

    PostProcessEffect::lazyInitialize(camera);

    BufferTexture* renderTarget = camera->getRenderTargetBuffer();
    uint32_t w = renderTarget->width() >> DOWNSAMPLE;
    uint32_t h = renderTarget->height() >> DOWNSAMPLE;

    for (size_t i = 0; i < m_Buffers.size(); i++)
        m_Buffers[i] = new BufferTexture(w, h, /*depth=*/ 0, TextureFormat::RGBAHalf);

    m_Material->setTexture("u_ShaftsTex", m_Buffers[0]->colorTexture());
    m_Material->setColor("u_Color", Color(1.0f, 0.91f, 0.8f) * 1.5f);

    m_DepthFilterMat->setTexture("u_Depth", camera->getDepthTexture());
    m_DepthFilterMat->setFloat("u_Threshold", 0.9f);
}

void SunShafts::render(BufferTexture* source, BufferTexture* destination)
{
    Vector3 sunDir = m_Camera->getTransform()->getPosition() - ((m_SunTransform) ? m_SunTransform->getForward() : Vector3::forward);
    Vector3 sunViewportPos = m_Camera->worldToViewportPoint(sunDir);
    m_DepthFilterMat->setVector3("u_SunPos", sunViewportPos);
    PostProcessEffect::render(source, m_Buffers[0], m_DepthFilterMat);

    m_RadialBlurMat->setVector3("u_SunPos", sunViewportPos);
    const float UNIT_SIZE = 1.0f / 512.0f;

    for (size_t i = 0; i < BLUR_ITERATIONS; i++)
    {
        m_RadialBlurMat->setFloat("u_BlurDist", (i * 2.0f + 1.0f) * UNIT_SIZE * BLUR_RADIUS);
        PostProcessEffect::render(m_Buffers[0], m_Buffers[1], m_RadialBlurMat);
        m_RadialBlurMat->setFloat("u_BlurDist", (i * 2.0f + 2.0f) * UNIT_SIZE * BLUR_RADIUS);
        PostProcessEffect::render(m_Buffers[1], m_Buffers[0], m_RadialBlurMat);
    }

    PostProcessEffect::render(source, destination);
}
