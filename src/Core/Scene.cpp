#include "Scene.h"

Scene::Scene()
{
	// Setup camera
	m_Camera = new Camera(
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3::zero,
		70.0f,		// FOV
		0.1f,		// Near
		1000.0f		// Far
	);

	// Load and compile shaders
	Shader* rainbow = new Shader("res/shaders/Rainbow.shader");

	// Create basic materials for objects below
	Material* matA = new Material(rainbow);

	for (int i = 0; i < 100; i++)
	{
		Entity* entity = new Entity(Vector3(-0.65f, 0.45f, 2.95f), Vector3(35.0f, 60.0f, -25.0f), Vector3(0.85f, 0.6f, 0.75f));
		entity->setupRenderer(MeshPrimitives::cube, matA);
		m_Entities.push_back(entity);
	}
}

Scene::~Scene()
{
	delete m_Camera;

	for (size_t i = 0; i < m_Entities.size(); i++)
	{
		delete m_Entities[i];
	}

	m_Entities.clear();
}

void Scene::update()
{
	double t = Time::GetTime();
	double dt = Time::GetDeltaTime();

	for (size_t i = 0; i < m_Entities.size(); i++)
	{
		t += 0.317529 * i;
		Entity* entity = m_Entities[i];

		entity->getTransform()->setPosition(Vector3(
			(float)sin(t * 0.5) * 1.0f,
			(float)cos(t * 0.6) * 0.5f,
			(float)sin(t * 0.1) * 7.0f + 9.0f
		));
		entity->getTransform()->setRotation(rotationToRad(Vector3(
			(float)t * 25.0f,
			(float)t * 30.0f,
			(float)t * 35.0f
		)));
		entity->getTransform()->setScale(Vector3(
			(float)sin(t * 1.5) * 0.03f + 0.45f,
			(float)cos(t * 1.6) * 0.05f + 0.45f,
			(float)sin(t * 1.7) * 0.03f + 0.45f
		));
	}

	if (m_Camera)
	{
		// Mouse look
		Vector2 mouseDelta = Input::GetMouseMoveDelta() * 0.075f;
		Vector3 rotateDelta = rotationToRad(Vector3(mouseDelta.y, mouseDelta.x, 0.0f));
		m_Camera->getTransform()->rotate(rotateDelta);

		// Free fly
		const float MOVE_SPEED = 3.0f;
		Vector3 moveDelta = getMoveAxis() * MOVE_SPEED * (float)dt;
		m_Camera->getTransform()->translate(moveDelta, Space::Local);

		bool space = Input::GetKey(KeyCode::Space);
		bool ctrl = Input::GetKey(KeyCode::LeftCtrl) || Input::GetKey(KeyCode::RightCtrl);
		float y = 0.0f;

		if (space && !ctrl)
		{
			y = 1.0f;
		}
		else if (ctrl && !space)
		{
			y = -1.0f;
		}

		m_Camera->getTransform()->translate(Vector3(0.0f, y, 0.0f) * MOVE_SPEED * (float)dt, Space::World);

		// Update and upload view projection.
		m_Camera->update();
	}
}

void Scene::draw()
{
	for (size_t i = 0; i < m_Entities.size(); i++)
	{
		m_Entities[i]->draw(*m_Camera);
	}
}

Vector3 Scene::getMoveAxis() const
{
	bool w = Input::GetKey(KeyCode::W);
	bool s = Input::GetKey(KeyCode::S);
	bool a = Input::GetKey(KeyCode::A);
	bool d = Input::GetKey(KeyCode::D);

	float x = 0.0f, z = 0.0f;

	if (d && !a)
	{
		x = 1.0f;
	}
	else if (a && !d)
	{
		x = -1.0f;
	}

	if (w && !s)
	{
		z = 1.0f;
	}
	else if (s && !w)
	{
		z = -1.0f;
	}

	return Vector3(x, 0.0f, z);
}
