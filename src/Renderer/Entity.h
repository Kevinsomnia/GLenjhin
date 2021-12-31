#pragma once

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
class Light;
class Material;
class Mesh;
class MeshRenderer;


class Entity
{
public:
    Entity(const Vector3& position, const Vector3& rotation, const Vector3& scale);
    ~Entity();
    void setupRenderer(Mesh* mesh, Material* material);
    void earlyUpdate();
    void drawGeometryPass(Material& geometryMat) const;
    void drawShadowPass(Material& shadowMat) const;
    void draw(const Camera& camera, const std::vector<Light*>& lights) const;

    Transform* transform() const { return m_Transform; }
    MeshRenderer* renderer() const { return m_Renderer; }
private:
    Transform* m_Transform;
    MeshRenderer* m_Renderer;
};
