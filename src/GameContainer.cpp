#include "GameContainer.h"

GameContainer::GameContainer() : time(0.0), m_currentScene(nullptr)
{
	// Load scene.
	m_currentScene = new Scene();
}

GameContainer::~GameContainer()
{
}

void GameContainer::update(double deltaTime)
{
	time += deltaTime;
}

void GameContainer::render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (m_currentScene)
	{
		m_currentScene->draw();
	}
}
