#include "MeshRenderer.h"
#include "../Core/Time.h"
#include "../Math/Convert.h"
#include "../Math/Matrix4x4.h"
#include "../Math/Vector.h"

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) : m_Mesh(mesh), m_Material(material)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::draw(const Matrix4x4& pos, const Matrix4x4& rot, const Matrix4x4& scale)
{
	if (m_Mesh)
	{
		if (m_Material)
		{
			m_Material->setMatrix("mPosition", pos);
			m_Material->setMatrix("mRotation", rot);
			m_Material->setMatrix("mScale", scale);
			m_Material->bind();
		}

		m_Mesh->bind();
		m_Mesh->draw();
		m_Mesh->unbind();

		if (m_Material)
			m_Material->unbind();
	}
}
