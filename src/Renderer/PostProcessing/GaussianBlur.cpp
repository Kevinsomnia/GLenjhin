#include "GaussianBlur.h"

GaussianBlur::GaussianBlur() : ImageEffect("res\\shaders\\PostProcessing\\Common\\Copy.glsl")
{
    const int SCR_WIDTH = 1600;
    const int SCR_HEIGHT = 900;

    const int WIDTH = SCR_WIDTH >> DOWNSAMPLE;
    const int HEIGHT = SCR_HEIGHT >> DOWNSAMPLE;

    for (size_t i = 0; i < m_Buffers.size(); i++)
        m_Buffers[i] = new BufferTexture(WIDTH, HEIGHT, /*depth=*/ 0, TextureFormat::RGB24);

    m_DownsampleMat = new Material(new Shader("res\\shaders\\PostProcessing\\GaussianBlur\\Downsample.glsl"));
    m_BlurMat = new Material(new Shader("res\\shaders\\PostProcessing\\GaussianBlur\\Blur.glsl"));
}

GaussianBlur::~GaussianBlur()
{
    for (size_t i = 0; i < m_Buffers.size(); i++)
        delete m_Buffers[i];
}

void GaussianBlur::render(BufferTexture* source, BufferTexture* destination)
{
    Vector2 screenTexelSize = source->texelSize();
    m_DownsampleMat->setVector("u_TexelSize", screenTexelSize);

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

        m_BlurMat->setVector("u_Stride", strideHorizontal);
        ImageEffect::render(bt1, bt2, m_BlurMat);
        m_BlurMat->setVector("u_Stride", strideVertical);

        if (i == BLUR_ITERATIONS - 1)
        {
            // Target the destination buffer / screen for the last blit.
            glViewport(0, 0, source->width(), source->height());
            bt1 = destination;
        }

        ImageEffect::render(bt2, bt1, m_BlurMat);
    }
}
