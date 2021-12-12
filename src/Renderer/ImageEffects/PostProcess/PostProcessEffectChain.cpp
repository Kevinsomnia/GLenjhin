#include "PostProcessEffectChain.h"

PostProcessEffectChain::PostProcessEffectChain(Camera* camera) : m_Camera(camera), m_NumColorBuffers(0)
{
    m_CopyMat = new Material(new Shader("res\\shaders\\ImageEffects\\Common\\Copy.glsl"));
    m_Triangle = new FullscreenTriangle(m_CopyMat);
}

PostProcessEffectChain::~PostProcessEffectChain()
{
    for (size_t i = 0; i < m_NumColorBuffers; i++)
        delete m_ColorBuffers[i];

    delete m_CopyMat;
    delete m_Triangle;
}

void PostProcessEffectChain::add(PostProcessEffect* effect)
{
    effect->lazyInitialize(m_Camera);
    m_Effects.push_back(effect);

    // Initialize swap buffers if necessary for reading/writing. Amount we need depends on how many effects we add, but can only be at most 2.
    size_t numBuffersRequired = Min(m_Effects.size(), (size_t)2);
    BufferTexture* cameraTex = m_Camera->getRenderTargetBuffer();

    while (m_NumColorBuffers < numBuffersRequired)
        m_ColorBuffers[m_NumColorBuffers++] = new BufferTexture(cameraTex->width(), cameraTex->height(), /*depth=*/ 0, TextureFormat::RGBAHalf);
}

void PostProcessEffectChain::render()
{
    BufferTexture* source = m_Camera->getRenderTargetBuffer();
    bool pingPongFlag = false;

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

    if (m_Effects.size() > 0)
    {
        // Copy final color buffer back into `source`, as long as we processed at least one effect.
        source->bind();
        m_CopyMat->setTexture("u_MainTex", m_ColorBuffers[pingPongFlag]->colorTexture());
        m_Triangle->draw();
    }
}
