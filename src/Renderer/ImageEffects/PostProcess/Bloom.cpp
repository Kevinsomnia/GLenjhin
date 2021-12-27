#include "Bloom.h"

Bloom::Bloom() : PostProcessEffect("res\\shaders\\ImageEffects\\Bloom\\Combine.glsl")
{
    m_PrefilterMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\Bloom\\Prefilter.glsl"));
    m_DownsampleMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\Bloom\\Downsample.glsl"));
    m_UpsampleMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\Bloom\\Upsample.glsl"));

    m_PrefilterMat->setFloat("u_Threshold", 1.25f);
    m_PrefilterMat->setFloat("u_SoftKnee", 0.5f);
}

Bloom::~Bloom()
{
    delete m_PrefilterMat;
    delete m_DownsampleMat;
    delete m_UpsampleMat;
}

void Bloom::render(BufferTexture* source, BufferTexture* destination)
{
    // Setup buffers.
    uint32_t w = source->width();
    uint32_t h = source->height();

    m_BufferCount = 0;
    for (size_t i = 0; i < MAX_BUFFER_COUNT; i++)
    {
        m_Buffers[i] = BufferTexturePool::Get(w, h, /*colorFormat=*/ TextureFormat::RGBAHalf, /*depthFormat=*/ TextureFormat::None);
        m_BufferCount++;
        w >>= 1;
        h >>= 1;

        if (w <= 1 || h <= 1)
            break;
    }

    // Pre-filter and isolate bright pixels.
    m_PrefilterMat->setVector2("u_TexelSize", source->texelSize());
    PostProcessEffect::render(source, m_Buffers[0], m_PrefilterMat);

    BufferTexture* currBuffer = nullptr;
    BufferTexture* nextBuffer = nullptr;

    // Downsample w/ 13-tap filter
    for (size_t i = 0; i < m_BufferCount - 1; i++)
    {
        currBuffer = m_Buffers[i];
        nextBuffer = m_Buffers[i + 1];

        m_DownsampleMat->setVector2("u_TexelSize", currBuffer->texelSize());

        PostProcessEffect::render(currBuffer, nextBuffer, m_DownsampleMat);
    }

    // Upsample w/ 9-tap tent filter.
    for (size_t i = m_BufferCount - 1; i >= 2; i--)
    {
        currBuffer = m_Buffers[i];
        nextBuffer = m_Buffers[i - 1];

        m_UpsampleMat->setTexture("u_PrevTex", currBuffer->colorTexture());
        m_UpsampleMat->setVector2("u_BlurSize", currBuffer->texelSize() * BLUR_SIZE);

        PostProcessEffect::render(currBuffer, nextBuffer, m_UpsampleMat);
    }

    // Upsample final bloom texture and accumulate color to screen.
    currBuffer = m_Buffers[1];
    m_Material->setTexture("u_ScreenTex", source->colorTexture());
    m_Material->setVector2("u_BlurSize", currBuffer->texelSize() * BLUR_SIZE);
    PostProcessEffect::render(currBuffer, destination);

    for (size_t i = 0; i < m_BufferCount; i++)
        BufferTexturePool::Return(m_Buffers[i]);
}
