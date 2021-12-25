#include "PostProcessEffect.h"

PostProcessEffect::PostProcessEffect(const std::string& shaderPath) : m_Initialized(false)
{
    m_Shader = new Shader(shaderPath);
    m_Material = new Material(m_Shader);
}

PostProcessEffect::~PostProcessEffect()
{
    delete m_Shader;
    delete m_Material;
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
    destination->bind();
    FullscreenTriangle::Draw(*mat, /*depthTest=*/ false);
}

void PostProcessEffect::render(BufferTexture* source, BufferTexture* destination, Material* mat) const
{
    // Copy `source` to `destination` FBO using `mat`.
    mat->setTexture("u_MainTex", source->colorTexture());
    render(destination, mat);
}
