#include "Lighting.h"

DirectionalLight::DirectionalLight(const Vector3& pos, const Vector3& rot)
    : Light(pos, rot)
{
}

void DirectionalLight::bind(Material* mat) const
{
    Light::bind(mat);

    if (mat)
    {
        Vector3 fwd = m_Transform->getTRS().multiplyVector(Vector3::forward);
        mat->setVector("u_DirLightDir", fwd);
    }
}
