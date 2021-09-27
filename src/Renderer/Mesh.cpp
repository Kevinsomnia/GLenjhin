#include "Mesh.h"

Mesh::Mesh(const Vertex* vertices, const uint32_t vertexCount, const uint32_t* indices, const uint32_t indexCount)
	: m_Vertices(vertices), m_VertexCount(vertexCount), m_Indices(indices), m_TriangleCount(indexCount / 3), m_VaoId(0), m_IboId(0)
{
	if (indexCount % 3 != 0)
	{
		cout << "Warning: index count is not divisible by 3!" << endl;
	}

	// Create and bind a vertex array
	glGenVertexArrays(1, &m_VaoId);
	glBindVertexArray(m_VaoId);

	// Create a vertex buffer
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// Vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);
	glEnableVertexAttribArray(0);

	// Create and bind a index buffer
	glGenBuffers(1, &m_IboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

Mesh::~Mesh()
{
}

void Mesh::bind()
{
	glBindVertexArray(m_VaoId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IboId);
}

void Mesh::unbind()
{
	glBindVertexArray(NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}

void Mesh::draw()
{
	glDrawElements(GL_TRIANGLES, m_TriangleCount * 3, GL_UNSIGNED_INT, nullptr);
}
