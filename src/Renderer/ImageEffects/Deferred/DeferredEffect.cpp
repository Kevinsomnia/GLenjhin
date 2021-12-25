#include "DeferredEffect.h"

DeferredEffect::DeferredEffect(const std::string& shaderPath) : m_Initialized(false)
{
    m_Shader = new Shader(shaderPath);
    m_Material = new Material(m_Shader);
    m_CopyMat = new Material(new Shader("res\\shaders\\ImageEffects\\Common\\Copy.glsl"));
}

DeferredEffect::~DeferredEffect()
{
    delete m_Shader;
    delete m_Material;
}

void DeferredEffect::lazyInitialize(Camera* camera)
{
    if (m_Initialized)
        return;

    m_Initialized = true;
    m_Camera = camera;
}

void DeferredEffect::render()
{
}

void DeferredEffect::render(BufferTexture* destination, Material* mat) const
{
    // Use `mat` directly to output to `destination` FBO.
    destination->bind();
    FullscreenTriangle::Draw(mat, /*depthTest=*/ false);
}

void DeferredEffect::render(BufferTexture* source, BufferTexture* destination, Material* mat) const
{
    // Copy `source` to `destination` FBO using `mat`.
    mat->setTexture("u_MainTex", source->colorTexture());
    render(destination, mat);
}
