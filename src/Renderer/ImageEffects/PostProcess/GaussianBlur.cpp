#include "GaussianBlur.h"

GaussianBlur::GaussianBlur() : PostProcessEffect("res\\shaders\\ImageEffects\\Common\\Copy.glsl")
{
    m_DownsampleMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\GaussianBlur\\Downsample.glsl"));
    m_BlurMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\GaussianBlur\\Blur.glsl"));
}

GaussianBlur::~GaussianBlur()
{
    delete m_DownsampleMat;
    delete m_BlurMat;
}

void GaussianBlur::render(BufferTexture* source, BufferTexture* destination)
{
    Vector2 screenTexelSize = source->texelSize();
    m_DownsampleMat->setVector2("u_TexelSize", screenTexelSize);

    uint32_t w = source->width() >> DOWNSAMPLE;
    uint32_t h = source->height() >> DOWNSAMPLE;
    BufferTexture* bt0 = BufferTexturePool::Get(w, h, /*colorFormat=*/ TextureFormat::RGBAHalf, /*depthFormat=*/ TextureFormat::None);
    BufferTexture* bt1 = BufferTexturePool::Get(w, h, /*colorFormat=*/ TextureFormat::RGBAHalf, /*depthFormat=*/ TextureFormat::None);

    // Downsample and filter
    PostProcessEffect::render(source, bt0, m_DownsampleMat);

    float blurFactor = BLUR_RADIUS * (source->height() / REFERENCE_HEIGHT);
    Vector2 strideHorizontal = Vector2(screenTexelSize.x * blurFactor, 0.0f);
    Vector2 strideVertical = Vector2(0.0f, screenTexelSize.y * blurFactor);

    for (int i = 0; i < BLUR_ITERATIONS; i++)
    {
        m_BlurMat->setVector2("u_Stride", strideHorizontal);
        PostProcessEffect::render(bt0, bt1, m_BlurMat);
        m_BlurMat->setVector2("u_Stride", strideVertical);

        if (i == BLUR_ITERATIONS - 1)
        {
            // Target the destination buffer / screen for the last blit.
            BufferTexturePool::Return(bt0);
            bt0 = destination;
        }

        PostProcessEffect::render(bt1, bt0, m_BlurMat);
    }

    if (bt0 != destination)
        BufferTexturePool::Return(bt0);
    BufferTexturePool::Return(bt1);
}
