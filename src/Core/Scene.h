#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>
#include "../Interaction/Input.h"
#include "../Renderer/Camera.h"
#include "../Renderer/Entity.h"
#include "../Renderer/MeshPrimitives.h"
#include "../Renderer/Skybox.h"
#include "../Renderer/Lighting/Lighting.h"
#include "../Renderer/Texture/Texture.h"
#include "Time.h"

using std::cout;
using std::endl;

class Scene
{
public:
    Scene();
    ~Scene();
    void update();
    void draw(const Camera& camera, bool drawSkybox = true);
    void setNewTexture(const std::string& texturePath);
private:
    Skybox* m_Skybox;
    Material* m_CurrMat;
    Texture2D* m_CurrTexture;
    std::vector<Entity*> m_Entities;
    std::vector<Entity*> m_DynamicEntities;
    std::vector<Light*> m_Lights;
};

#endif // SCENE_H
