#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) : m_Mesh(mesh), m_Material(material)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::draw(const Vector3& cameraPos, const Matrix4x4& vp, const Matrix4x4& model, const std::vector<Light*>& lights)
{
	if (m_Mesh)
	{
		if (m_Material)
		{
			m_Material->setMatrix("u_VP", vp);
			m_Material->setMatrix("u_Model", model);
			m_Material->setVector("u_CameraPos", cameraPos);
			m_Material->bind();
		}

		// NOTE: multiple lights of same type are not supported!
		for (Light* light : lights)
			light->bind(m_Material);

		m_Mesh->bind();
		m_Mesh->draw();
	}
}
