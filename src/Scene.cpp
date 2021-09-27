#include "Scene.h"

Scene::Scene()
{
	// Load and compile shaders
	Shader* unlit = new Shader("res/shaders/Unlit.shader");
	Shader* rainbow = new Shader("res/shaders/Rainbow.shader");

	// Create basic materials for objects below
	Material* matA = new Material(rainbow);
	Material* matB = new Material(unlit);

	// Create 2 simple mesh objects
	const unsigned int numVerticesA = 5;
	Vertex verticesA[numVerticesA] = {
		Vertex(Vec3(0.0f, 0.0f, 0.0f)),
		Vertex(Vec3(1.0f, 0.25f, 1.0f)),
		Vertex(Vec3(0.5f, 0.9f, 0.5f)),
		Vertex(Vec3(-0.5f, 0.8f, 0.5f)),
		Vertex(Vec3(-1.0f, 0.1f, 0.0f))
	};
	const unsigned int numIndicesA = 9;
	unsigned int indicesA[numIndicesA] = {
		0, 1, 2,
		2, 3, 0,
		0, 3, 4
	};
	Mesh* meshA = new Mesh(verticesA, numVerticesA, indicesA, numIndicesA);

	const unsigned int numVerticesB = 4;
	Vertex verticesB[numVerticesB] = {
		Vertex(Vec3(-0.25f, -0.75f, 0.0f)),
		Vertex(Vec3(-0.25f, -0.25f, 0.5f)),
		Vertex(Vec3(-0.75f, -0.3f, 0.0f)),
		Vertex(Vec3(-0.75f, -0.6f, 0.5f))
	};
	const unsigned int numIndicesB = 6;
	unsigned int indicesB[numIndicesB] = {
		0, 1, 2,
		2, 3, 0,
	};
	Mesh* meshB = new Mesh(verticesB, numVerticesB, indicesB, numIndicesB);

	m_Renderers.push_back(new MeshRenderer(meshA, matA));
	m_Renderers.push_back(new MeshRenderer(meshB, matB));
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
