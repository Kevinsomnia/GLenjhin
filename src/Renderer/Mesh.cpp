#include "Mesh.h"

Mesh::Mesh()
    : m_Vertices(nullptr), m_VertexCount(0), m_Indices(nullptr), m_TriangleCount(0), m_VaoId(0), m_IboId(0)
{
}

Mesh::Mesh(Vertex* vertices, size_t vertexCount, uint32_t* indices, size_t indexCount)
    : m_Vertices(nullptr), m_VertexCount(0), m_Indices(nullptr), m_TriangleCount(0), m_VaoId(0), m_IboId(0)
{
    setup(vertices, vertexCount, indices, indexCount);
}

Mesh::~Mesh()
{
}

void Mesh::bind() const
{
    glBindVertexArray(m_VaoId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IboId);
}

void Mesh::unbind() const
{
    glBindVertexArray(NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}

void Mesh::draw() const
{
    glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(m_TriangleCount) * 3, GL_UNSIGNED_INT, nullptr);
}

void Mesh::setup(Vertex* vertices, size_t vertexCount, uint32_t* indices, size_t indexCount)
{
    m_Vertices = vertices;
    m_VertexCount = vertexCount;
    m_Indices = indices;
    m_TriangleCount = indexCount / 3;

    // Create and bind a vertex array
    glGenVertexArrays(1, &m_VaoId);
    glBindVertexArray(m_VaoId);
    uint32_t stride = sizeof(Vertex);

    // Create a vertex buffer
    uint32_t vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * stride, vertices, GL_STATIC_DRAW);

    // Vertex position
    size_t offset = 0;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
    glEnableVertexAttribArray(0);
    offset += sizeof(Vector3);
    // Vertex normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
    glEnableVertexAttribArray(1);
    offset += sizeof(Vector3);
    // UVs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
    glEnableVertexAttribArray(2);
    offset += sizeof(Vector2);

    // Create and bind a index buffer
    glGenBuffers(1, &m_IboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}
