#include "GlobalFog.h"

GlobalFog::GlobalFog() : ImageEffect("res\\shaders\\PostProcessing\\GlobalFog.shader")
{
}

GlobalFog::~GlobalFog()
{
}

void GlobalFog::render(BufferTexture* source, BufferTexture* destination)
{    
    m_Material->setMatrix("u_VP", m_Camera->getViewProjMatrix());
    m_Material->setVector("u_ProjectionParams", m_Camera->getProjectionParams());
    m_Material->setTexture("u_DepthTex", m_Camera->getGBuffers()->depthTexture());
    ImageEffect::render(source, destination);
}
