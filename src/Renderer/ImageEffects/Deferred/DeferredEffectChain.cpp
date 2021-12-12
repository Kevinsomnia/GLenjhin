#include "DeferredEffectChain.h"

DeferredEffectChain::DeferredEffectChain(Camera* camera) : m_Camera(camera)
{
    m_CopyMat = new Material(new Shader("res\\shaders\\ImageEffects\\Common\\Copy.glsl"));
    m_Triangle = new FullscreenTriangle(m_CopyMat);
}

DeferredEffectChain::~DeferredEffectChain()
{
    delete m_CopyMat;
    delete m_Triangle;
}

void DeferredEffectChain::add(DeferredEffect* effect)
{
    effect->lazyInitialize(m_Camera);
    m_Effects.push_back(effect);

    // Other initialization stuff
}

void DeferredEffectChain::render()
{
    GeometryBuffers* gBuffers = m_Camera->getGBuffers();

    for (size_t i = 0; i < m_Effects.size(); i++)
    {
        DeferredEffect* effect = m_Effects[i];
    }
}
