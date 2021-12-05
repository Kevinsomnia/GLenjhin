#include "Lighting.h"

Light::Light(const Vector3& pos, const Quaternion& rot)
{
    m_Transform = new Transform(pos, rot, Vector3::one);
    m_ShadowMat = new Material(new Shader("res\\shaders\\Lighting\\ShadowPass.glsl"));
}
