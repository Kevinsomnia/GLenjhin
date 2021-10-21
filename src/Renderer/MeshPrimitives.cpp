#include "MeshPrimitives.h"

MeshPrimitives::Cube::Cube() : Mesh()
{
    std::array<Vertex, 24> vertices = {
        // +Z
        Vertex(Vector3(-0.5f, 0.5f, 0.5f), Vector3::forward, Vector2(1.0f, 1.0f)),
        Vertex(Vector3(0.5f, 0.5f, 0.5f), Vector3::forward, Vector2(0.0f, 1.0f)),
        Vertex(Vector3(-0.5f, -0.5f, 0.5f), Vector3::forward, Vector2(1.0f, 0.0f)),
        Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector3::forward, Vector2(0.0f, 0.0f)),

        // -Z
        Vertex(Vector3(-0.5f, 0.5f, -0.5f), Vector3::back, Vector2(0.0f, 1.0f)),
        Vertex(Vector3(0.5f, 0.5f, -0.5f), Vector3::back, Vector2(1.0f, 1.0f)),
        Vertex(Vector3(-0.5f, -0.5f, -0.5f), Vector3::back, Vector2(0.0f, 0.0f)),
        Vertex(Vector3(0.5f, -0.5f, -0.5f), Vector3::back, Vector2(1.0f, 0.0f)),

        // +X
        Vertex(Vector3(0.5f, 0.5f, 0.5f), Vector3::right, Vector2(1.0f, 1.0f)),
        Vertex(Vector3(0.5f, 0.5f, -0.5f), Vector3::right, Vector2(0.0f, 1.0f)),
        Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector3::right, Vector2(1.0f, 0.0f)),
        Vertex(Vector3(0.5f, -0.5f, -0.5f), Vector3::right, Vector2(0.0f, 0.0f)),

        // -X
        Vertex(Vector3(-0.5f, 0.5f, 0.5f), Vector3::left, Vector2(0.0f, 1.0f)),
        Vertex(Vector3(-0.5f, 0.5f, -0.5f), Vector3::left, Vector2(1.0f, 1.0f)),
        Vertex(Vector3(-0.5f, -0.5f, 0.5f), Vector3::left, Vector2(0.0f, 0.0f)),
        Vertex(Vector3(-0.5f, -0.5f, -0.5f), Vector3::left, Vector2(1.0f, 0.0f)),

        // +Y
        Vertex(Vector3(-0.5f, 0.5f, 0.5f), Vector3::up, Vector2(0.0f, 1.0f)),
        Vertex(Vector3(0.5f, 0.5f, 0.5f), Vector3::up, Vector2(1.0f, 1.0f)),
        Vertex(Vector3(-0.5f, 0.5f, -0.5f), Vector3::up, Vector2(0.0f, 0.0f)),
        Vertex(Vector3(0.5f, 0.5f, -0.5f), Vector3::up, Vector2(1.0f, 0.0f)),

        // -Y
        Vertex(Vector3(-0.5f, -0.5f, 0.5f), Vector3::down, Vector2(1.0f, 1.0f)),
        Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector3::down, Vector2(0.0f, 1.0f)),
        Vertex(Vector3(-0.5f, -0.5f, -0.5f), Vector3::down, Vector2(1.0f, 0.0f)),
        Vertex(Vector3(0.5f, -0.5f, -0.5f), Vector3::down, Vector2(0.0f, 0.0f)),
    };
    std::array<uint32_t, 12*3> indices {
        // +Z
        0, 1, 3,
        0, 3, 2,
        // -Z
        4, 7, 5,
        4, 6, 7,
        // +X
        8, 9, 11,
        8, 11, 10,
        // -X
        12, 15, 13,
        12, 14, 15,
        // +Y
        16, 19, 17,
        16, 18, 19,
        // -Y
        20, 21, 23,
        20, 23, 22
    };
    setup(vertices.data(), vertices.size(), indices.data(), indices.size());
}

MeshPrimitives::Quad::Quad() : Mesh()
{
    // Single-sided quad that faces +Z
    std::array<Vertex, 4> vertices = {
        Vertex(Vector3(-0.5f, 0.5f, 0.0f), Vector3::forward, Vector2(1.0f, 1.0f)),
        Vertex(Vector3(0.5f, 0.5f, 0.0f), Vector3::forward, Vector2(0.0f, 1.0f)),
        Vertex(Vector3(-0.5f, -0.5f, 0.0f), Vector3::forward, Vector2(1.0f, 0.0f)),
        Vertex(Vector3(0.5f, -0.5f, 0.0f), Vector3::forward, Vector2(0.0f, 0.0f))
    };
    std::array<uint32_t, 2*3> indices = {
        0, 1, 3,
        0, 3, 2
    };
    setup(vertices.data(), vertices.size(), indices.data(), indices.size());
}

MeshPrimitives::Cube* MeshPrimitives::cube = nullptr;
MeshPrimitives::Quad* MeshPrimitives::quad = nullptr;

void MeshPrimitives::Init()
{
    cube = new Cube();
    quad = new Quad();
}
