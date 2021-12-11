#include "GaussianBlur.h"

GaussianBlur::GaussianBlur() : ImageEffect("res\\shaders\\ImageEffects\\Common\\Copy.glsl")
{
    m_DownsampleMat = new Material(new Shader("res\\shaders\\ImageEffects\\GaussianBlur\\Downsample.glsl"));
    m_BlurMat = new Material(new Shader("res\\shaders\\ImageEffects\\GaussianBlur\\Blur.glsl"));
}

GaussianBlur::~GaussianBlur()
{
    delete m_DownsampleMat;
    delete m_BlurMat;

    if (m_Initialized)
    {
        for (size_t i = 0; i < m_Buffers.size(); i++)
            delete m_Buffers[i];
    }
}

void GaussianBlur::lazyInitialize(Camera* camera)
{
    if (m_Initialized)
        return;

    ImageEffect::lazyInitialize(camera);

    BufferTexture* bufferTex = camera->getRenderTargetBuffer();
    int w = bufferTex->width() >> DOWNSAMPLE;
    int h = bufferTex->height() >> DOWNSAMPLE;

    for (size_t i = 0; i < m_Buffers.size(); i++)
        m_Buffers[i] = new BufferTexture(w, h, /*depth=*/ 0, TextureFormat::RGBA32);
}

void GaussianBlur::render(BufferTexture* source, BufferTexture* destination)
{
    Vector2 screenTexelSize = source->texelSize();
    m_DownsampleMat->setVector2("u_TexelSize", screenTexelSize);

    // Downsample
    BufferTexture* downsampledBuffer = m_Buffers[0];
    glViewport(0, 0, downsampledBuffer->width(), downsampledBuffer->height());
    ImageEffect::render(source, downsampledBuffer, m_DownsampleMat);

    float blurFactor = BLUR_RADIUS * (source->height() / REFERENCE_HEIGHT);
    Vector2 strideHorizontal = Vector2(screenTexelSize.getX() * blurFactor, 0.0f);
    Vector2 strideVertical = Vector2(0.0f, screenTexelSize.getY() * blurFactor);

    for (int i = 0; i < BLUR_ITERATIONS; i++)
    {
        BufferTexture* bt1 = m_Buffers[0];
        BufferTexture* bt2 = m_Buffers[1];
        glViewport(0, 0, bt1->width(), bt1->height());

        m_BlurMat->setVector2("u_Stride", strideHorizontal);
        ImageEffect::render(bt1, bt2, m_BlurMat);
        m_BlurMat->setVector2("u_Stride", strideVertical);

        if (i == BLUR_ITERATIONS - 1)
        {
            // Target the destination buffer / screen for the last blit.
            glViewport(0, 0, source->width(), source->height());
            bt1 = destination;
        }

        ImageEffect::render(bt2, bt1, m_BlurMat);
    }
}
