#include "ImageEffectChain.h"

ImageEffectChain::ImageEffectChain(Camera* camera, ImageEffectChainType type) : m_Camera(camera), m_Type(type)
{
    m_CopyMat = new Material(new Shader("res\\shaders\\PostProcessing\\Common\\Copy.glsl"));
    m_Triangle = new FullscreenTriangle(m_CopyMat);

    if (m_Type == ImageEffectChainType::PostProcess)
    {
        BufferTexture* cameraTex = camera->getRenderTargetBuffer();

        // Create 2 color buffers for ping-ponging, since we can't read and write to the same buffer when iterating through image effects.
        // TODO: only initialize 1 buffer if we don't have more than one effect, or any buffers at all if there are no image effects (add logic in ImageEffectChain::add).
        for (size_t i = 0; i < m_ColorBuffers.size(); i++)
            m_ColorBuffers[i] = new BufferTexture(cameraTex->width(), cameraTex->height(), /*depth=*/ 0, TextureFormat::RGBAHalf);
    }
    else if (m_Type == ImageEffectChainType::Deferred)
    {

    }
}

ImageEffectChain::~ImageEffectChain()
{
    for (size_t i = 0; i < m_ColorBuffers.size(); i++)
        delete m_ColorBuffers[i];

    delete m_CopyMat;
    delete m_Triangle;
}

void ImageEffectChain::add(ImageEffect* effect)
{
    effect->lazyInitialize(m_Camera);
    m_Effects.push_back(effect);
}

void ImageEffectChain::render()
{
    if (m_Type == ImageEffectChainType::PostProcess)
    {
        BufferTexture* source = m_Camera->getRenderTargetBuffer();
        bool pingPongFlag = false;

        for (size_t i = 0; i < m_Effects.size(); i++)
        {
            ImageEffect* effect = m_Effects[i];

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
            glViewport(0, 0, source->width(), source->height());
            glBindFramebuffer(GL_FRAMEBUFFER, source->id());
            m_CopyMat->setTexture("u_MainTex", m_ColorBuffers[pingPongFlag]->colorTexture());
            m_Triangle->draw();
        }
    }
    else if (m_Type == ImageEffectChainType::Deferred)
    {
    }
}
