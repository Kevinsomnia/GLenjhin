#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <GL/glew.h>

using std::cout;
using std::endl;

class Mesh
{
public:
	Mesh(const float* vertices, const unsigned int vertexCount, const unsigned int* indices, const unsigned int indexCount);
	~Mesh();
	void bind();
	void unbind();
	void draw();
private:
	const float* vertices;
	const unsigned int vertexCount;
	const unsigned int* indices;
	const unsigned int triangleCount;
	unsigned int m_vaoId;
	unsigned int m_iboId;
};

#endif // MESH_H
