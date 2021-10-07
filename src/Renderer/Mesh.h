#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <GL/glew.h>
#include "../Math/Vector.h"

using std::cout;
using std::endl;

struct Vertex
{
    Vertex(const Vector3& position, const Vector3& normal, const Vector2& uv)
        : position(position), normal(normal), uv(uv) {}

    Vector3 position;
    Vector3 normal;
    Vector2 uv;
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
