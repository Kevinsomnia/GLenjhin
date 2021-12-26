#include "Lighting.h"

Light::Light(const Vector3& pos, const Vector3& rot)
{
    m_Transform = new Transform(pos, rot, Vector3::one);
    m_ShadowMat = new Material(Shader::Load("res\\shaders\\Lighting\\ShadowPass.glsl"));
}
