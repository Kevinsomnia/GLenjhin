#include "Bloom.h"

Bloom::Bloom() : ImageEffect("res\\shaders\\PostProcessing\\Bloom\\Combine.shader")
{
    const int SCR_WIDTH = 1600;
    const int SCR_HEIGHT = 900;

    int tw = SCR_WIDTH;
    int th = SCR_HEIGHT;

    for (size_t i = 0; i < MAX_BUFFER_COUNT; i++)
    {
        m_Buffers.push_back(new BufferTexture(tw, th, /*depth=*/ 0, TextureFormat::RGBAHalf));
        tw >>= 1;
        th >>= 1;

        if (tw <= 1 || th <= 1)
            break;
    }

    m_PrefilterMat = new Material(new Shader("res\\shaders\\PostProcessing\\Bloom\\Prefilter.shader"));
    m_DownsampleMat = new Material(new Shader("res\\shaders\\PostProcessing\\Bloom\\Downsample.shader"));
    m_UpsampleMat = new Material(new Shader("res\\shaders\\PostProcessing\\Bloom\\Upsample.shader"));
}

Bloom::~Bloom()
{
}

void Bloom::render(BufferTexture* source, BufferTexture* destination)
{
    // Pre-filter and isolate bright pixels.
    m_PrefilterMat->setVector("u_TexelSize", source->texelSize());
    ImageEffect::render(source, m_Buffers[0], m_PrefilterMat);

    BufferTexture* currBuffer = nullptr;
    BufferTexture* nextBuffer = nullptr;

    // Downsample w/ 13-tap filter
    for (size_t i = 0; i < m_Buffers.size() - 1; i++)
    {
        currBuffer = m_Buffers[i];
        nextBuffer = m_Buffers[i + 1];

        m_DownsampleMat->setVector("u_TexelSize", currBuffer->texelSize());

        glViewport(0, 0, nextBuffer->width(), nextBuffer->height());
        ImageEffect::render(currBuffer, nextBuffer, m_DownsampleMat);
    }

    // Upsample w/ 9-tap tent filter.
    for (size_t i = m_Buffers.size() - 1; i >= 2; i--)
    {
        currBuffer = m_Buffers[i];
        nextBuffer = m_Buffers[i - 1];

        m_UpsampleMat->setTexture("u_PrevTex", currBuffer->colorTexture());
        m_UpsampleMat->setVector("u_BlurSize", currBuffer->texelSize() * BLUR_SIZE);

        glViewport(0, 0, nextBuffer->width(), nextBuffer->height());
        ImageEffect::render(currBuffer, nextBuffer, m_UpsampleMat);
    }

    // Upsample final bloom texture and accumulate color to screen.
    currBuffer = m_Buffers[1];
    m_Material->setTexture("u_ScreenTex", source->colorTexture());
    m_Material->setVector("u_BlurSize", currBuffer->texelSize() * BLUR_SIZE);
    glViewport(0, 0, source->width(), source->height());
    ImageEffect::render(currBuffer, destination);
}