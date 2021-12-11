#include "ImageEffect.h"

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
