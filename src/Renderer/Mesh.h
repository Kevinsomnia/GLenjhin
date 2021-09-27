#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <GL/glew.h>
#include "../Math/Vector.h"

using std::cout;
using std::endl;

struct Vertex
{
	Vertex(Vec3 position)
		: position(position) {}

	Vec3 position;
};

class Mesh
{
public:
	Mesh(const Vertex* vertices, const unsigned int vertexCount, const unsigned int* indices, const unsigned int indexCount);
	~Mesh();
	void bind();
	void unbind();
	void draw();
private:
	const Vertex* m_Vertices;
	const unsigned int m_VertexCount;
	const unsigned int* m_Indices;
	const unsigned int m_TriangleCount;
	unsigned int m_VaoId;
	unsigned int m_IboId;
};

#endif // MESH_H
