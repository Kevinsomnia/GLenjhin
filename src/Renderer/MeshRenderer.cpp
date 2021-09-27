#include "MeshRenderer.h"

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
			m_Material->bind();

		m_Mesh->bind();
		m_Mesh->draw();
		m_Mesh->unbind();

		if (m_Material)
			m_Material->unbind();
	}
}
