#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include "../Math/Convert.h"
#include "../Math/Vector.h"
#include "MeshRenderer.h"

using std::cout;
using std::cerr;
using std::endl;

class Entity
{
public:
	Entity(const Vector3& position, const Vector3& rotation, const Vector3& scale);
	~Entity();
	void setupRenderer(Mesh* mesh, Material* material);
	void draw() const;

	Vector3 getPosition() const;
	Vector3 getRotation() const;
	Vector3 getScale() const;
	void setPosition(const Vector3& position);
	void setRotation(const Vector3& rotation);
	void setScale(const Vector3& scale);
	const MeshRenderer* getRenderer() const;
private:
	MeshRenderer* m_Renderer;
	Vector3 m_Position;
	Vector3 m_Rotation;
	Vector3 m_Scale;
};

#endif // ENTITY_H
