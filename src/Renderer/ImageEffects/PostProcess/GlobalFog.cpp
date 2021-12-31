#include "GlobalFog.h"

GlobalFog::GlobalFog() : PostProcessEffect("res\\shaders\\ImageEffects\\GlobalFog.glsl")
{
}

GlobalFog::~GlobalFog()
{
}

void GlobalFog::render(BufferTexture* source, BufferTexture* destination)
{    
    m_Material->setMatrix4x4("u_VP", m_Camera->viewProjectionMatrix());
    m_Material->setVector4("u_ProjectionParams", m_Camera->projectionParams());
    m_Material->setTexture("u_DepthTex", m_Camera->depthTexture());
    PostProcessEffect::render(source, destination);
}
