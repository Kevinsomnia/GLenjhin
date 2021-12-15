#pragma once

#include <glad/glad.h>
#include "../Math/Vector.h"

using std::cout;
using std::endl;

struct Vertex
{
    Vertex(const Vector3& position, const Vector3& normal, const Vector2& uv)
        : position(position), normal(normal), uv(uv) {}
    Vertex(const Vector3& position, const Vector3& normal, const Vector3& tangent, const Vector2& uv) : Vertex(position, normal, uv)
    {
        this->tangent = tangent;
    }

    Vector3 position;
    Vector3 normal;
    Vector3 tangent;
    Vector2 uv;
};

class Mesh
{
public:
    Mesh();
    Mesh(Vertex* vertices, size_t vertexCount, uint32_t* indices, size_t indexCount);
    ~Mesh();
    void bind() const;
    void unbind() const;
    void draw() const;
    size_t vertexCount() const { return m_VertexCount; }
    size_t triangleCount() const { return m_IndexCount / 3; }
    void setGeometry(Vertex* vertices, size_t vertexCount, uint32_t* indices, size_t indexCount);
    void calculateTangents();
    void updateBuffers();
protected:
    Vertex* m_Vertices;
    size_t m_VertexCount;
    uint32_t* m_Indices;
    size_t m_IndexCount;
    uint32_t m_VaoId;
    uint32_t m_VboId;
    uint32_t m_IboId;
};
