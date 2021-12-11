#include "Tonemapping.h"

Tonemapping::Tonemapping() : ImageEffect("res\\shaders\\ImageEffects\\Tonemapping.glsl")
{
    m_Material->setFloat("u_Exposure", 3.25f);
}

Tonemapping::~Tonemapping()
{
}
