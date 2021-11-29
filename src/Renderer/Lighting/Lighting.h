#ifndef LIGHTING_H
#define LIGHTING_H

#include <iostream>

#include "../../Math/Math.h"
#include "../Material.h"
#include "../Transform.h"

using std::cout;
using std::endl;

class Light
{
public:
    virtual void bind(Material& mat) const;
protected:
    Light(const Vector3& pos, const Vector3& rot);
    Transform* m_Transform;
};

class DirectionalLight : public Light
{
    void bind(Material& mat) const override;
public:
    DirectionalLight(const Vector3& pos, const Vector3& rot);
};

#endif
