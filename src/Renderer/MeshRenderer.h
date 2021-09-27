#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <iostream>
#include <GL/glew.h>
#include "Material.h"
#include "Mesh.h"

using std::cout;
using std::endl;

class MeshRenderer
{
public:
	MeshRenderer(Mesh* mesh, Material* material);
	~MeshRenderer();
	void draw();
private:
	Mesh* m_Mesh;
	Material* m_Material;
};

#endif // MESHRENDERER_H
