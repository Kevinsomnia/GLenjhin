#include "Entity.h"

Entity::Entity(const Vector3& position, const Vector3& rotation, const Vector3& scale)
	: m_Renderer(nullptr)
{
	m_Transform = new Transform(position, rotation, scale);
}

Entity::~Entity()
{
	delete m_Transform;

	if (m_Renderer)
	{
		delete m_Renderer;
		m_Renderer = nullptr;
	}
}

void Entity::setupRenderer(Mesh* mesh, Material* material)
{
	if (m_Renderer)
	{
		cerr << "Renderer is already set up!" << endl;
		return;
	}

	m_Renderer = new MeshRenderer(mesh, material);
}

void Entity::draw(const Camera& cam, const std::vector<Light*>& lights) const
{
	if (m_Renderer)
	{
		m_Renderer->draw(
			cam.getViewProjMatrix(),
			m_Transform->getTRS(),
			lights
		);
	}
}

Transform* Entity::getTransform() const
{
	return m_Transform;
}

MeshRenderer* Entity::getRenderer() const
{
	return m_Renderer;
}
