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

void MeshRenderer::draw(const Matrix4x4& view, const Matrix4x4& model)
{
	if (m_Mesh)
	{
		if (m_Material)
		{
			m_Material->setMatrix("u_View", view);
			m_Material->setMatrix("u_Model", model);
			m_Material->bind();
		}

		m_Mesh->bind();
		m_Mesh->draw();
		m_Mesh->unbind();

		if (m_Material)
			m_Material->unbind();
	}
}
