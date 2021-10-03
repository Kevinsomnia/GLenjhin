#include "Entity.h"

Entity::Entity(const Vector3& position, const Vector3& rotation, const Vector3& scale)
	: m_Renderer(nullptr), m_Position(position), m_Rotation(rotation), m_Scale(scale)
{
}

Entity::~Entity()
{
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
		m_Renderer->draw(Matrix4x4::TRS(
			m_Position,
			Vector3(degToRad(m_Rotation.x), degToRad(m_Rotation.y), degToRad(m_Rotation.z)),
			m_Scale
		));
	}
}

Vector3 Entity::getPosition() const
{
	return m_Position;
}

Vector3 Entity::getRotation() const
{
	return m_Rotation;
}

Vector3 Entity::getScale() const
{
	return m_Scale;
}

void Entity::setPosition(const Vector3& position)
{
	m_Position = position;
}

void Entity::setRotation(const Vector3& rotation)
{
	m_Rotation = rotation;
}

void Entity::setScale(const Vector3& scale)
{
	m_Scale = scale;
}

const MeshRenderer* Entity::getRenderer() const
{
	return m_Renderer;
}
