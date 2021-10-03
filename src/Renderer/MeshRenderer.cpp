#include "MeshRenderer.h"
#include "../Math/Convert.h"
#include "../Math/Matrix4x4.h"
#include "../Math/Vector.h"

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) : m_Mesh(mesh), m_Material(material)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::draw()
{
	if (m_Mesh)
	{
		if (m_Material)
		{
			m_Material->setMatrix("mPosition", Matrix4x4::Translate(Vector3(-0.85f, -1.85f, 3.5f)));
			m_Material->setMatrix("mRotation", Matrix4x4::Rotate(Vector3(degToRad(60.0f), degToRad(35.0f), degToRad(15.0f))));
			m_Material->setMatrix("mScale", Matrix4x4::Scale(Vector3(1.4f, 1.0f, 0.7f)));
			m_Material->bind();
		}

		m_Mesh->bind();
		m_Mesh->draw();
		m_Mesh->unbind();

		if (m_Material)
			m_Material->unbind();
	}
}
