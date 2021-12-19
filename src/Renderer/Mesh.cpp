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
    // Vertex tangent
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
    glEnableVertexAttribArray(2);
    offset += sizeof(Vector3);
    // UVs
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
    glEnableVertexAttribArray(3);
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

void Mesh::calculateTangents()
{
    if (!m_Vertices)
        return;

    // Clear all tangent data.
    for (size_t i = 0; i < m_VertexCount; i++)
        m_Vertices[i].tangent = Vector3::zero;

    // Process each triangle by stepping through every 3 indices.
    for (size_t i = 0; i < m_IndexCount; i += 3)
    {
        uint32_t index0 = m_Indices[i];
        uint32_t index1 = m_Indices[i + 1];
        uint32_t index2 = m_Indices[i + 2];

        Vertex& vert0 = m_Vertices[index0];
        Vertex& vert1 = m_Vertices[index1];
        Vertex& vert2 = m_Vertices[index2];

        // Calculate 2 edges of this triangle (2 vectors originating from same point) in 3D and UV space.
        Vector3 xyzEdge0 = vert1.position - vert0.position;
        Vector3 xyzEdge1 = vert2.position - vert0.position;
        Vector2 uvEdge0 = vert1.uv - vert0.uv;
        Vector2 uvEdge1 = vert2.uv - vert0.uv;

        // Accumulate tangents since multiple triangles can share the same vertex.
        Vector3 tangent = (xyzEdge0 * uvEdge1.y - xyzEdge1 * uvEdge0.y) / (uvEdge0.x * uvEdge1.y - uvEdge1.x * uvEdge0.y);
        vert0.tangent += tangent;
        vert1.tangent += tangent;
        vert2.tangent += tangent;
    }

    // Normalize.
    for (size_t i = 0; i < m_VertexCount; i++)
        m_Vertices[i].tangent.normalize();
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
