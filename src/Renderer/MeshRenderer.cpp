#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Mesh* mesh) : mesh(mesh)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::draw()
{
	if (mesh)
	{
		mesh->bind();
		mesh->draw();
		mesh->unbind();
	}
}
