#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <GL/glew.h>
#include "../Math/Vector.h"

using std::cout;
using std::endl;

struct Vertex
{
	Vertex(Vector3 position)
		: position(position) {}

	Vector3 position;
};

class Mesh
{
public:
	Mesh();
	Mesh(Vertex* vertices, uint32_t vertexCount, uint32_t* indices, uint32_t indexCount);
	~Mesh();
	void bind() const;
	void unbind() const;
	void draw() const;
protected:
	Vertex* m_Vertices;
	uint32_t m_VertexCount;
	uint32_t* m_Indices;
	uint32_t m_TriangleCount;
	uint32_t m_VaoId;
	uint32_t m_IboId;

	void setup(Vertex* vertices, uint32_t vertexCount, uint32_t* indices, uint32_t indexCount);
};

#endif // MESH_H
