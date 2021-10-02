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
		4, 7, 5,
		4, 6, 7,
		// +Z
		0, 1, 3,
		0, 3, 2,
		// -X
		2, 4, 0,
		2, 6, 4,
		// +X
		5, 3, 1,
		5, 7, 3,
		// -Y
		7, 2, 3,
		7, 6, 2,
		// +Y
		0, 5, 1,
		0, 4, 5
	};
	Mesh* meshA = new Mesh(verticesA, numVerticesA, indicesA, numIndicesA);

	m_Renderers.push_back(new MeshRenderer(meshA, matA));
}

Scene::~Scene()
{
	for (size_t i = 0; i < m_Renderers.size(); i++)
	{
		delete m_Renderers[i];
	}

	m_Renderers.clear();
}

void Scene::draw()
{
	for (size_t i = 0; i < m_Renderers.size(); i++)
	{
		m_Renderers[i]->draw();
	}
}
