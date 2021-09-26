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
	Mesh* mesh;
	Material* material;
};

#endif // MESHRENDERER_H
