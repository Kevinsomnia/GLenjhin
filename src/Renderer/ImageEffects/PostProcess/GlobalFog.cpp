#include "GlobalFog.h"

GlobalFog::GlobalFog() : PostProcessEffect("res\\shaders\\ImageEffects\\GlobalFog.glsl")
{
}

GlobalFog::~GlobalFog()
{
}

void GlobalFog::render(BufferTexture* source, BufferTexture* destination)
{    
    m_Material->setMatrix("u_VP", m_Camera->getViewProjectionMatrix());
    m_Material->setVector4("u_ProjectionParams", m_Camera->getProjectionParams());
    m_Material->setTexture("u_DepthTex", m_Camera->getDepthTexture());
    PostProcessEffect::render(source, destination);
}
