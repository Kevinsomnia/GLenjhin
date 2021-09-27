#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>
#include "Renderer/MeshRenderer.h"

using std::cout;
using std::endl;

class Scene
{
public:
	Scene();
	~Scene();
	void draw();
private:
	std::vector<MeshRenderer*> m_Renderers;
};

#endif // SCENE_H
