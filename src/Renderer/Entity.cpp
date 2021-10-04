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

void Entity::draw() const
{
	if (m_Renderer)
	{
		Vector3 rot = m_Transform->getRotation();
		m_Renderer->draw(
			Matrix4x4::Orthographic(5.0f, 16.0f / 9.0f, 5.0f, 50.0f),
			Matrix4x4::View(
				Vector3(0.0f, 3.05f, 0.0f),
				Vector3(degToRad(35.0f), degToRad(-10.0f), degToRad(0.0f))
			),
			Matrix4x4::TRS(
				m_Transform->getPosition(),
				Vector3(degToRad(rot.x), degToRad(rot.y), degToRad(rot.z)),
				m_Transform->getScale()
			)
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
