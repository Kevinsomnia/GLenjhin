#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>
#include "../Renderer/Camera.h"
#include "../Renderer/Entity.h"
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
};

#endif // SCENE_H
