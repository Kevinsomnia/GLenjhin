#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include "../Math/Convert.h"
#include "../Math/Vector.h"
#include "Lighting/Lighting.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Transform.h"

using std::cout;
using std::cerr;
using std::endl;

class Entity
{
public:
    Entity(const Vector3& position, const Vector3& rotation, const Vector3& scale);
    ~Entity();
    void setupRenderer(Mesh* mesh, Material* material);
    void draw(const Camera& camera, const std::vector<Light*>& lights) const;

    Transform* getTransform() const;
    MeshRenderer* getRenderer() const;
private:
    Transform* m_Transform;
    MeshRenderer* m_Renderer;
};

#endif // ENTITY_H
