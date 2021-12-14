#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) : m_Mesh(mesh), m_Material(material)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::drawMeshDirect() const
{
    m_Mesh->bind();
    m_Mesh->draw();
}

void MeshRenderer::draw(const Vector3& cameraPos, const Matrix4x4& vp, const Matrix4x4& model, const std::vector<Light*>& lights) const
{
    if (m_Material)
    {
        Material& mat = *m_Material;
        mat.setMatrix4x4("u_VP", vp);
        mat.setMatrix4x4("u_Model", model);
        mat.setVector3("u_CameraPos", cameraPos);
        
        // NOTE: multiple lights of same type are not supported!
        for (Light* light : lights)
            light->setUniforms(mat);

        mat.bind();
    }

    drawMeshDirect();
}
