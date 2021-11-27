#include "PostProcessing.h"

ImageEffect::ImageEffect(const std::string& shaderPath)
{
    m_Shader = new Shader(shaderPath);
    m_Material = new Material(m_Shader);
    m_Triangle = new FullscreenTriangle(nullptr);
}

ImageEffect::~ImageEffect()
{
    delete m_Shader;
    delete m_Material;
    delete m_Triangle;
}

void ImageEffect::render(BufferTexture* source, BufferTexture* destination)
{
    render(source, destination, m_Material);
}

void ImageEffect::render(BufferTexture* source, BufferTexture* destination, Material* mat)
{
    // Render `readBufferTex` to quad and output to writeBuffer FBO.
    glBindFramebuffer(GL_FRAMEBUFFER, destination ? destination->id() : NULL);
    mat->setTexture("u_MainTex", source->colorTexture());
    m_Triangle->setMaterial(mat);
    m_Triangle->draw();
}


ImageEffectChain::ImageEffectChain()
{
    // Create 2 color buffers for ping-ponging, since we can't read and write to the same buffer when iterating through image effects.
    for (size_t i = 0; i < m_ColorBuffers.size(); i++)
        m_ColorBuffers[i] = new BufferTexture(1600, 900, /*depth=*/ 0, TextureFormat::RGBAHalf);
}

ImageEffectChain::~ImageEffectChain()
{
    for (size_t i = 0; i < m_ColorBuffers.size(); i++)
        delete m_ColorBuffers[i];
}

void ImageEffectChain::add(ImageEffect* effect)
{
    m_Effects.push_back(effect);
}

void ImageEffectChain::render(BufferTexture* screen)
{
    bool pingPongFlag = false;

    for (size_t i = 0; i < m_Effects.size(); i++)
    {
        ImageEffect* effect = m_Effects[i];

        if (effect)
        {
            bool isLast = i == m_Effects.size() - 1;

            if (i == 0)
            {
                // If there's only 1 effect, blit from quad and render directly to screen FBO.
                // If there's more than 1 effect, render to the first FBO.
                effect->render(screen, isLast ? nullptr : m_ColorBuffers[pingPongFlag]);
            }
            else
            {
                // Blit from current FBO to the other FBO and swap. If it's the last effect, then blit to screen FBO.
                effect->render(m_ColorBuffers[pingPongFlag], isLast ? nullptr : m_ColorBuffers[!pingPongFlag]);
                pingPongFlag = !pingPongFlag;
            }
        }
    }
}
