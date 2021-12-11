#include "PostProcessing.h"

ImageEffect::ImageEffect(const std::string& shaderPath) : m_Initialized(false)
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

void ImageEffect::lazyInitialize(Camera* camera)
{
    if (m_Initialized)
        return;

    m_Initialized = true;
    m_Camera = camera;
}

void ImageEffect::render(BufferTexture* source, BufferTexture* destination)
{
    render(source, destination, m_Material);
}

void ImageEffect::render(BufferTexture* source, BufferTexture* destination, Material* mat)
{
    // Render `source` to quad and output to `destination` FBO.
    glViewport(0, 0, destination->width(), destination->height());
    glBindFramebuffer(GL_FRAMEBUFFER, destination->id());

    if (source)
        mat->setTexture("u_MainTex", source->colorTexture());

    m_Triangle->setMaterial(mat);
    m_Triangle->draw();
}



ImageEffectChain::ImageEffectChain(Camera* camera) : m_Camera(camera)
{
    m_CopyMat = new Material(new Shader("res\\shaders\\PostProcessing\\Common\\Copy.glsl"));
    m_Triangle = new FullscreenTriangle(m_CopyMat);

    BufferTexture* cameraTex = camera->getRenderTargetBuffer();

    // Create 2 color buffers for ping-ponging, since we can't read and write to the same buffer when iterating through image effects.
    // TODO: only initialize 1 buffer if we don't have more than one effect, or any buffers at all if there are no image effects (add logic in ImageEffectChain::add).
    for (size_t i = 0; i < m_ColorBuffers.size(); i++)
        m_ColorBuffers[i] = new BufferTexture(cameraTex->width(), cameraTex->height(), /*depth=*/ 0, TextureFormat::RGBAHalf);
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

void ImageEffectChain::render(BufferTexture* source)
{
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
