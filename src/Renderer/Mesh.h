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
	const float* m_Vertices;
	const unsigned int m_VertexCount;
	const unsigned int* m_Indices;
	const unsigned int m_TriangleCount;
	unsigned int m_VaoId;
	unsigned int m_IboId;
};

#endif // MESH_H
