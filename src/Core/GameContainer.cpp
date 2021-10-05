#include "GameContainer.h"

GameContainer::GameContainer(GLFWwindow* window) : m_MainWindow(window), m_CurrentScene(nullptr)
{
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	// Enable back face culling
	glEnable(GL_CULL_FACE);

	Time::Init();
	Input::Init(window);

	// Load scene.
	m_CurrentScene = new Scene();
}

GameContainer::~GameContainer()
{
}

void GameContainer::update(double deltaTime)
{
	Time::Timestep(deltaTime);
	Input::Poll();

	if (m_CurrentScene)
		m_CurrentScene->update();
}

void GameContainer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.7f, 0.825f, 1.0f);

	if (m_CurrentScene)
		m_CurrentScene->draw();
}
