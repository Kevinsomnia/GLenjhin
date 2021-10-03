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

void MeshRenderer::draw()
{
	if (m_Mesh)
	{
		if (m_Material)
		{
			double t = Time::getTime();
			float x = (float)sin(t * 0.5) * 0.5f - 0.65f;
			float y = (float)cos(t * 0.6) * 0.5f - 0.45f;
			float z = (float)sin(t * 0.7) * (float)cos(t * 0.8) * 0.5f + 2.95f;

			float xTheta = (float)fmod(t * 60.0f, 360.0f);
			float yTheta = (float)fmod(t * 80.0f, 360.0f);
			float zTheta = (float)fmod(t * 100.0f, 360.0f);

			float x1 = (float)sin(t * 1.5) * 0.4f + 0.85f;
			float y1 = (float)cos(t * 1.6) * 0.3f - 0.65f;
			float z1 = (float)sin(t * 1.7) * (float)cos(t * 1.3) * 0.4f + 0.75f;

			m_Material->setMatrix("mPosition", Matrix4x4::Translate(Vector3(x, y, z)));
			m_Material->setMatrix("mRotation", Matrix4x4::Rotate(Vector3(degToRad(xTheta), degToRad(yTheta), degToRad(zTheta))));
			m_Material->setMatrix("mScale", Matrix4x4::Scale(Vector3(x1, y1, z1)));
			m_Material->bind();
		}

		m_Mesh->bind();
		m_Mesh->draw();
		m_Mesh->unbind();

		if (m_Material)
			m_Material->unbind();
	}
}
