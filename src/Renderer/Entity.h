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


// Forward declaration
class Camera;


class Entity
{
public:
    Entity(const Vector3& position, const Vector3& rotation, const Vector3& scale);
    ~Entity();
    void setupRenderer(Mesh* mesh, Material* material);
    void drawGeometryPass(const Camera& camera, Material& geometryMat) const;
    void draw(const Camera& camera, const std::vector<Light*>& lights) const;

    Transform* getTransform() const { return m_Transform; }
    MeshRenderer* getRenderer() const { return m_Renderer; }
private:
    Transform* m_Transform;
    MeshRenderer* m_Renderer;
};

#endif // ENTITY_H
