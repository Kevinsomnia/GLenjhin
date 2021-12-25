#pragma once

#include <vector>
#include "../Interaction/Input.h"
#include "../IO/ModelLib.h"
#include "../Renderer/Entity.h"
#include "../Renderer/MeshPrimitives.h"
#include "../Renderer/Skybox.h"
#include "../Renderer/Lighting/Lighting.h"
#include "Time.h"

using std::cout;
using std::endl;
using ModelLib::Model;
using ModelLib::ModelImporter;


// Forward declaration
class Camera;
class Entity;
class Light;


class Scene
{
public:
    Scene();
    ~Scene();
    void update();
    void drawGeometryPass(const Camera& camera, Material& geometryMat) const;
    void drawShadowPass(const Light& light, Material& shadowMat) const;
    void drawSkybox(const Camera& camera) const;
    void drawEntities(const Camera& camera) const;
    void renderLighting(Material& lightingMat) const;
    void renderLightShadows() const;
    void setNewTexture(const std::string& texturePath);
    std::vector<Light*>& lights() { return m_Lights; };
private:
    Skybox* m_Skybox;
    Model m_CustomModel;
    Model m_DragonModel;
    Material* m_GroundMat;
    Texture2D* m_GroundAlbedo;
    std::vector<Entity*> m_Entities;
    std::vector<Entity*> m_DynamicEntities;
    std::vector<Entity*> m_FastEntities;
    const std::vector<Light*> m_EmptyLightsList;  // An empty list for deferred path.
    std::vector<Light*> m_Lights;

    void userUpdate();  // User code (e.g. update entity transforms)
};
