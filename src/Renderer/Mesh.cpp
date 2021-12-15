#include "Mesh.h"

Mesh::Mesh()
    : m_Vertices(nullptr), m_VertexCount(0), m_Indices(nullptr), m_IndexCount(0), m_VaoId(0), m_IboId(0)
{
    // Allocate vertex array, vertex buffer, index buffer.
    glGenVertexArrays(1, &m_VaoId);
    glGenBuffers(1, &m_VboId);
    glGenBuffers(1, &m_IboId);

    // Setup the vertex layout.
    glBindVertexArray(m_VaoId);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboId);

    uint32_t stride = sizeof(Vertex);

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
}

Mesh::Mesh(Vertex* vertices, size_t vertexCount, uint32_t* indices, size_t indexCount) : Mesh()
{
    setGeometry(vertices, vertexCount, indices, indexCount);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_VaoId);
    glDeleteBuffers(1, &m_VboId);
    glDeleteBuffers(1, &m_IboId);
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
    glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(m_IndexCount), GL_UNSIGNED_INT, nullptr);
}

void Mesh::setGeometry(Vertex* vertices, size_t vertexCount, uint32_t* indices, size_t indexCount)
{
    m_Vertices = vertices;
    m_VertexCount = vertexCount;
    m_Indices = indices;
    m_IndexCount = indexCount;
}

void Mesh::updateBuffers()
{
    // Update vertex and index data
    uint32_t stride = sizeof(Vertex);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
    glBufferData(GL_ARRAY_BUFFER, m_VertexCount * stride, m_Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * sizeof(uint32_t), m_Indices, GL_STATIC_DRAW);
}
