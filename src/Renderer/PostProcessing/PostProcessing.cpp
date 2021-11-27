#include "PostProcessing.h"

ImageEffect::ImageEffect(const std::string& shaderPath)
{
    m_Shader = new Shader(shaderPath);
    m_Material = new Material(m_Shader);
    m_Triangle = new FullscreenTriangle(m_Material);
}

ImageEffect::~ImageEffect()
{
    delete m_Shader;
    delete m_Material;
    delete m_Triangle;
}

void ImageEffect::render(BufferTexture* readBufferTex, uint32_t writeBufferID)
{
    // Render `readBufferTex` to quad and output to writeBuffer FBO.
    glBindFramebuffer(GL_FRAMEBUFFER, writeBufferID);
    m_Material->setTexture("u_MainTex", readBufferTex);
    m_Material->bind();
    m_Triangle->draw();
}


ImageEffectChain::ImageEffectChain()
{
    const int WIDTH = 1600;
    const int HEIGHT = 900;

    // Create 2 color buffers for ping-ponging, since we can't read and write to the same buffer when iterating through image effects.
    glGenFramebuffers(2, m_FboIDs);

    for (int i = 0; i < NUM_BUFFERS; i++)
    {
        m_ColorBuffers[i] = new BufferTexture(WIDTH, HEIGHT, TextureFormat::RGBAHalf);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FboIDs[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBuffers[i]->id(), 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            cerr << "Post-processing framebuffer (RGBAHalf) failed to initialize." << endl;
            glDeleteFramebuffers(1, &m_FboIDs[i]);
            m_FboIDs[i] = NULL;
            delete m_ColorBuffers[i];
            m_ColorBuffers[i] = nullptr;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
}

ImageEffectChain::~ImageEffectChain()
{
    for (int i = 0; i < NUM_BUFFERS; i++)
    {
        glDeleteFramebuffers(1, &m_FboIDs[i]);
        m_FboIDs[i] = NULL;
        delete m_ColorBuffers[i];
    }
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
                effect->render(screen, isLast ? NULL : m_FboIDs[pingPongFlag]);
            }
            else
            {
                // Blit from current FBO to the other FBO and swap. If it's the last effect, then blit to screen FBO.
                effect->render(m_ColorBuffers[pingPongFlag], isLast ? NULL : m_FboIDs[!pingPongFlag]);
                pingPongFlag = !pingPongFlag;
            }
        }
    }
}
