#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) : mesh(mesh), material(material)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::draw()
{
	if (mesh)
	{
		if (material)
			material->bind();

		mesh->bind();
		mesh->draw();
		mesh->unbind();

		if (material)
			material->unbind();
	}
}
