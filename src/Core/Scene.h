#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>
#include "../Interaction/Input.h"
#include "../Renderer/Camera.h"
#include "../Renderer/Entity.h"
#include "../Renderer/MeshPrimitives.h"
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
    void draw();
private:
    Camera* m_Camera;
    std::vector<Entity*> m_Entities;
    std::vector<Light*> m_Lights;

    Vector3 getMoveAxis() const;
};

#endif // SCENE_H
