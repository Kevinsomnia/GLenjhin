#include "GameContainer.h"

GameContainer::GameContainer() : m_CurrentScene(nullptr)
{
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	// Enable back face culling
	glEnable(GL_CULL_FACE);

	Time::init();

	// Load scene.
	m_CurrentScene = new Scene();
}

GameContainer::~GameContainer()
{
}

void GameContainer::update(double deltaTime)
{
	Time::timestep(deltaTime);

	if (m_CurrentScene)
		m_CurrentScene->update();
}

void GameContainer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_CurrentScene)
		m_CurrentScene->draw();
}
