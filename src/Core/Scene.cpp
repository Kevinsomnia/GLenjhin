#include "Scene.h"

Scene::Scene()
{
	// Load and compile shaders
	Shader* rainbow = new Shader("res/shaders/Rainbow.shader");

	// Create basic materials for objects below
	Material* matA = new Material(rainbow);

	// Cube
	const uint32_t numVerticesA = 8;
	Vertex verticesA[numVerticesA] = {
		Vertex(Vector3(-0.5f, 0.5f, 0.5f)),
		Vertex(Vector3(0.5f, 0.5f, 0.5f)),
		Vertex(Vector3(-0.5f, -0.5f, 0.5f)),
		Vertex(Vector3(0.5f, -0.5f, 0.5f)),
		Vertex(Vector3(-0.5f, 0.5f, -0.5f)),
		Vertex(Vector3(0.5f, 0.5f, -0.5f)),
		Vertex(Vector3(-0.5f, -0.5f, -0.5f)),
		Vertex(Vector3(0.5f, -0.5f, -0.5f))
	};
	const uint32_t numIndicesA = 12*3;
	uint32_t indicesA[numIndicesA] = {
		// -Z
		4, 5, 7,
		4, 7, 6,
		// +Z
		0, 3, 1,
		0, 2, 3,
		// -X
		2, 0, 4,
		2, 4, 6,
		// +X
		5, 1, 3,
		5, 3, 7,
		// -Y
		7, 3, 2,
		7, 2, 6,
		// +Y
		0, 1, 5,
		0, 5, 4
	};
	Mesh* meshA = new Mesh(verticesA, numVerticesA, indicesA, numIndicesA);

	for (int i = 0; i < 100; i++)
	{
		Entity* entity = new Entity(Vector3(-0.65f, 0.45f, 2.95f), Vector3(35.0f, 60.0f, -25.0f), Vector3(0.85f, 0.6f, 0.75f));
		entity->setupRenderer(meshA, matA);
		m_Entities.push_back(entity);
	}
}

Scene::~Scene()
{
	for (size_t i = 0; i < m_Entities.size(); i++)
	{
		delete m_Entities[i];
	}

	m_Entities.clear();
}

void Scene::update()
{
	double t = Time::getTime();

	for (size_t i = 0; i < m_Entities.size(); i++)
	{
		t += 0.317529 * i;
		Entity* entity = m_Entities[i];

		entity->getTransform()->setPosition(Vector3(
			(float)sin(t * 0.5) * 4.0f,
			(float)cos(t * 0.6),
			(float)sin(t * 0.1) * 15.0f + 18.0f
		));
		entity->getTransform()->setRotation(Vector3(
			(float)t * 25.0f,
			(float)t * 30.0f,
			(float)t * 35.0f
		));
		entity->getTransform()->setScale(Vector3(
			(float)sin(t * 1.5) * 0.1f + 0.95f,
			(float)cos(t * 1.6) * 0.15f + 0.95f,
			(float)sin(t * 1.7) * 0.1f + 0.95f
		));
	}
}

void Scene::draw()
{
	for (size_t i = 0; i < m_Entities.size(); i++)
	{
		m_Entities[i]->draw();
	}
}
