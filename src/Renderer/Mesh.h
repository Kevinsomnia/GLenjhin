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
	Mesh(const Vertex* vertices, const uint32_t vertexCount, const uint32_t* indices, const uint32_t indexCount);
	~Mesh();
	void bind() const;
	void unbind() const;
	void draw() const;
private:
	const Vertex* m_Vertices;
	const uint32_t m_VertexCount;
	const uint32_t* m_Indices;
	const uint32_t m_TriangleCount;
	uint32_t m_VaoId;
	uint32_t m_IboId;
};

#endif // MESH_H
