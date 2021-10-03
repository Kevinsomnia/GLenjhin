#ifndef GAMECONTAINER_H
#define GAMECONTAINER_H

#include <iostream>
#include <GL/glew.h>
#include "Scene.h"
#include "Time.h"

using std::cout;
using std::endl;

class GameContainer
{
public:
	GameContainer();
	~GameContainer();
	void update(double deltaTime);
	void render();
private:
	Scene* m_CurrentScene;
};

#endif // GAMECONTAINER_H
