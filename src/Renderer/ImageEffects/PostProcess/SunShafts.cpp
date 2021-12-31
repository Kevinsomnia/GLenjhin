#include "SunShafts.h"

SunShafts::SunShafts() : PostProcessEffect("res\\shaders\\ImageEffects\\SunShafts\\Combine.glsl")
{
    m_DepthFilterMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\SunShafts\\DepthFilter.glsl"));
    m_RadialBlurMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\SunShafts\\RadialBlur.glsl"));
}

SunShafts::~SunShafts()
{
    delete m_DepthFilterMat;
    delete m_RadialBlurMat;
}

void SunShafts::render(BufferTexture* source, BufferTexture* destination)
{
    uint32_t w = source->width() >> DOWNSAMPLE;
    uint32_t h = source->height() >> DOWNSAMPLE;
    BufferTexture* bt0 = BufferTexturePool::Get(w, h, /*colorFormat=*/ TextureFormat::RGBAHalf, /*depthFormat=*/ TextureFormat::None);
    BufferTexture* bt1 = BufferTexturePool::Get(w, h, /*colorFormat=*/ TextureFormat::RGBAHalf, /*depthFormat=*/ TextureFormat::None);
    m_Material->setTexture("u_ShaftsTex", bt0->colorTexture());

    Vector3 sunDir = m_Camera->transform()->position() - ((m_SunTransform) ? m_SunTransform->forward() : Vector3::forward);
    Vector3 sunViewportPos = m_Camera->worldToViewportPoint(sunDir);
    m_DepthFilterMat->setTexture("u_Depth", m_Camera->depthTexture());
    m_DepthFilterMat->setVector3("u_SunPos", sunViewportPos);
    m_DepthFilterMat->setFloat("u_Threshold", 0.9f);
    PostProcessEffect::render(source, bt0, m_DepthFilterMat);

    m_RadialBlurMat->setVector3("u_SunPos", sunViewportPos);
    const float UNIT_SIZE = 1.0f / 512.0f;

    for (size_t i = 0; i < BLUR_ITERATIONS; i++)
    {
        m_RadialBlurMat->setFloat("u_BlurDist", (i * 2.0f + 1.0f) * UNIT_SIZE * BLUR_RADIUS);
        PostProcessEffect::render(bt0, bt1, m_RadialBlurMat);
        m_RadialBlurMat->setFloat("u_BlurDist", (i * 2.0f + 2.0f) * UNIT_SIZE * BLUR_RADIUS);
        PostProcessEffect::render(bt1, bt0, m_RadialBlurMat);
    }

    m_Material->setColor("u_Color", Color(1.0f, 0.91f, 0.8f) * 1.5f);
    PostProcessEffect::render(source, destination);

    BufferTexturePool::Return(bt0);
    BufferTexturePool::Return(bt1);
}
