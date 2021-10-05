#ifndef GAMECONTAINER_H
#define GAMECONTAINER_H

#include <iostream>
#include "../Interaction/Input.h"
#include "../Renderer/MeshPrimitives.h"
#include "Scene.h"
#include "Time.h"

using std::cout;
using std::endl;

class GameContainer
{
public:
	GameContainer(GLFWwindow* window);
	~GameContainer();
	void update(double deltaTime);
	void render();
private:
	GLFWwindow* m_MainWindow;
	Scene* m_CurrentScene;
};

#endif // GAMECONTAINER_H
