#include "PostProcessEffect.h"

PostProcessEffect::PostProcessEffect(const std::string& shaderPath) : m_Initialized(false)
{
    m_Shader = new Shader(shaderPath);
    m_Material = new Material(m_Shader);
    m_Triangle = new FullscreenTriangle(nullptr);
}

PostProcessEffect::~PostProcessEffect()
{
    delete m_Shader;
    delete m_Material;
    delete m_Triangle;
}

void PostProcessEffect::lazyInitialize(Camera* camera)
{
    if (m_Initialized)
        return;

    m_Initialized = true;
    m_Camera = camera;
}

void PostProcessEffect::render(BufferTexture* source, BufferTexture* destination)
{
    render(source, destination, m_Material);
}

void PostProcessEffect::render(BufferTexture* destination, Material* mat) const
{
    // Use `mat` directly to output to `destination` FBO.
    glViewport(0, 0, destination->width(), destination->height());
    glBindFramebuffer(GL_FRAMEBUFFER, destination->id());
    m_Triangle->setMaterial(mat);
    m_Triangle->draw();
}

void PostProcessEffect::render(BufferTexture* source, BufferTexture* destination, Material* mat) const
{
    // Copy `source` to `destination` FBO using `mat`.
    mat->setTexture("u_MainTex", source->colorTexture());
    render(destination, mat);
}
