#include "DeferredEffect.h"

DeferredEffect::DeferredEffect(const std::string& shaderPath) : m_Initialized(false)
{
    m_Shader = new Shader(shaderPath);
    m_Material = new Material(m_Shader);
    m_Triangle = new FullscreenTriangle(nullptr);
}

DeferredEffect::~DeferredEffect()
{
    delete m_Shader;
    delete m_Material;
    delete m_Triangle;
}

void DeferredEffect::lazyInitialize(Camera* camera)
{
    if (m_Initialized)
        return;

    m_Initialized = true;
    m_Camera = camera;
}

void DeferredEffect::render(BufferTexture* source, BufferTexture* destination)
{
    render(source, destination, m_Material);
}

void DeferredEffect::render(BufferTexture* source, BufferTexture* destination, Material* mat)
{
    // Use `mat` directly to output to `destination` FBO.
    destination->bind();

    if (source)
        mat->setTexture("u_MainTex", source->colorTexture());

    m_Triangle->setMaterial(mat);
    m_Triangle->draw();
}
