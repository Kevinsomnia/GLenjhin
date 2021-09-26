#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <iostream>
#include <GL/glew.h>
#include "Mesh.h"

using std::cout;
using std::endl;

class MeshRenderer
{
public:
	MeshRenderer(Mesh* mesh);
	~MeshRenderer();
	void draw();
private:
	Mesh* mesh;
};

#endif // MESHRENDERER_H
