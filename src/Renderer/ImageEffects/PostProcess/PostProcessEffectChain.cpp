#include "PostProcessEffectChain.h"

PostProcessEffectChain::PostProcessEffectChain(Camera* camera) : m_Camera(camera), m_NumColorBuffers(0)
{
    m_CopyMat = new Material(Shader::Load("res\\shaders\\ImageEffects\\Common\\Copy.glsl"));
}

PostProcessEffectChain::~PostProcessEffectChain()
{
    delete m_CopyMat;
}

void PostProcessEffectChain::add(PostProcessEffect* effect)
{
    effect->lazyInitialize(m_Camera);
    m_Effects.push_back(effect);

    // Calculate how many color buffers are needed to render to the camera's target.
    m_NumColorBuffers = Math::Min(m_Effects.size(), (size_t)2);
}

void PostProcessEffectChain::render()
{
    if (m_Effects.size() == 0)
        return;

    BufferTexture* source = m_Camera->getRenderTargetBuffer();
    bool pingPongFlag = false;

    for (size_t i = 0; i < m_NumColorBuffers; i++)
        m_ColorBuffers[i] = BufferTexturePool::Get(source->width(), source->height(), /*colorFormat=*/ TextureFormat::RGBAHalf, /*depthFormat=*/ TextureFormat::None);

    for (size_t i = 0; i < m_Effects.size(); i++)
    {
        PostProcessEffect* effect = m_Effects[i];

        if (i == 0)
        {
            // The first image effect will always blit from `source` to the first color buffer.
            effect->render(source, m_ColorBuffers[pingPongFlag]);
        }
        else
        {
            // Blit from `pingPongFlag` (read) FBO to the `!pingPongFlag` (write) FBO, then flip the bit.
            effect->render(m_ColorBuffers[pingPongFlag], m_ColorBuffers[!pingPongFlag]);
            pingPongFlag = !pingPongFlag;
        }
    }

    // Copy final color buffer back into `source`
    source->bind();
    m_CopyMat->setTexture("u_MainTex", m_ColorBuffers[pingPongFlag]->colorTexture());
    FullscreenTriangle::Draw(*m_CopyMat, /*depthTest=*/ false);

    for (size_t i = 0; i < m_NumColorBuffers; i++)
        BufferTexturePool::Return(m_ColorBuffers[i]);
}
