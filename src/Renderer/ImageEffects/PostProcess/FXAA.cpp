#include "FXAA.h"

FXAA::FXAA() : PostProcessEffect("res\\shaders\\ImageEffects\\FXAA.glsl")
{
}

FXAA::~FXAA()
{
}

void FXAA::lazyInitialize(Camera* camera)
{
    if (m_Initialized)
        return;

    PostProcessEffect::lazyInitialize(camera);
    m_Material->setVector2("u_TexelSize", camera->getRenderTargetBuffer()->texelSize());
}
