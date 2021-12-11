#include "Bloom.h"

Bloom::Bloom() : ImageEffect("res\\shaders\\ImageEffects\\Bloom\\Combine.glsl")
{
    m_PrefilterMat = new Material(new Shader("res\\shaders\\ImageEffects\\Bloom\\Prefilter.glsl"));
    m_DownsampleMat = new Material(new Shader("res\\shaders\\ImageEffects\\Bloom\\Downsample.glsl"));
    m_UpsampleMat = new Material(new Shader("res\\shaders\\ImageEffects\\Bloom\\Upsample.glsl"));

    m_PrefilterMat->setFloat("u_Threshold", 1.25f);
    m_PrefilterMat->setFloat("u_SoftKnee", 0.5f);
}

Bloom::~Bloom()
{
    delete m_PrefilterMat;
    delete m_DownsampleMat;
    delete m_UpsampleMat;

    if (m_Initialized)
    {
        for (size_t i = 0; i < m_Buffers.size(); i++)
            delete m_Buffers[i];
    }
}

void Bloom::lazyInitialize(Camera* camera)
{
    if (m_Initialized)
        return;

    ImageEffect::lazyInitialize(camera);

    BufferTexture* bufferTex = camera->getRenderTargetBuffer();
    int tw = bufferTex->width();
    int th = bufferTex->height();

    for (size_t i = 0; i < MAX_BUFFER_COUNT; i++)
    {
        m_Buffers.push_back(new BufferTexture(tw, th, /*depth=*/ 0, TextureFormat::RGBAHalf));
        tw >>= 1;
        th >>= 1;

        if (tw <= 1 || th <= 1)
            break;
    }
}

void Bloom::render(BufferTexture* source, BufferTexture* destination)
{
    // Pre-filter and isolate bright pixels.
    m_PrefilterMat->setVector2("u_TexelSize", source->texelSize());
    ImageEffect::render(source, m_Buffers[0], m_PrefilterMat);

    BufferTexture* currBuffer = nullptr;
    BufferTexture* nextBuffer = nullptr;

    // Downsample w/ 13-tap filter
    for (size_t i = 0; i < m_Buffers.size() - 1; i++)
    {
        currBuffer = m_Buffers[i];
        nextBuffer = m_Buffers[i + 1];

        m_DownsampleMat->setVector2("u_TexelSize", currBuffer->texelSize());

        glViewport(0, 0, nextBuffer->width(), nextBuffer->height());
        ImageEffect::render(currBuffer, nextBuffer, m_DownsampleMat);
    }

    // Upsample w/ 9-tap tent filter.
    for (size_t i = m_Buffers.size() - 1; i >= 2; i--)
    {
        currBuffer = m_Buffers[i];
        nextBuffer = m_Buffers[i - 1];

        m_UpsampleMat->setTexture("u_PrevTex", currBuffer->colorTexture());
        m_UpsampleMat->setVector2("u_BlurSize", currBuffer->texelSize() * BLUR_SIZE);

        glViewport(0, 0, nextBuffer->width(), nextBuffer->height());
        ImageEffect::render(currBuffer, nextBuffer, m_UpsampleMat);
    }

    // Upsample final bloom texture and accumulate color to screen.
    currBuffer = m_Buffers[1];
    m_Material->setTexture("u_ScreenTex", source->colorTexture());
    m_Material->setVector2("u_BlurSize", currBuffer->texelSize() * BLUR_SIZE);
    glViewport(0, 0, source->width(), source->height());
    ImageEffect::render(currBuffer, destination);
}
