#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) : m_Mesh(mesh), m_Material(material)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::draw(const Matrix4x4& vp, const Matrix4x4& model)
{
	if (m_Mesh)
	{
		if (m_Material)
		{
			m_Material->setMatrix("u_VP", vp);
			m_Material->setMatrix("u_Model", model);
			m_Material->bind();
		}

		m_Mesh->bind();
		m_Mesh->draw();
	}
}
