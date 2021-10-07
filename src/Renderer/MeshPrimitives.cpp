#include "MeshPrimitives.h"

MeshPrimitives::Cube::Cube() : Mesh()
{
    const uint32_t NUM_VERTICES = 24;
    Vertex vertices[NUM_VERTICES] = {
        // +Z
        Vertex(Vector3(-0.5f, 0.5f, 0.5f), Vector3::forward, Vector2(0.0f, 1.0f)),
        Vertex(Vector3(0.5f, 0.5f, 0.5f), Vector3::forward, Vector2(1.0f, 1.0f)),
        Vertex(Vector3(-0.5f, -0.5f, 0.5f), Vector3::forward, Vector2(0.0f, 0.0f)),
        Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector3::forward, Vector2(1.0f, 0.0f)),

        // -Z
        Vertex(Vector3(-0.5f, 0.5f, -0.5f), Vector3::back, Vector2(0.0f, 1.0f)),
        Vertex(Vector3(0.5f, 0.5f, -0.5f), Vector3::back, Vector2(1.0f, 1.0f)),
        Vertex(Vector3(-0.5f, -0.5f, -0.5f), Vector3::back, Vector2(0.0f, 0.0f)),
        Vertex(Vector3(0.5f, -0.5f, -0.5f), Vector3::back, Vector2(1.0f, 0.0f)),

        // +X
        Vertex(Vector3(0.5f, 0.5f, 0.5f), Vector3::right, Vector2(0.0f, 1.0f)),
        Vertex(Vector3(0.5f, 0.5f, -0.5f), Vector3::right, Vector2(1.0f, 1.0f)),
        Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector3::right, Vector2(0.0f, 0.0f)),
        Vertex(Vector3(0.5f, -0.5f, -0.5f), Vector3::right, Vector2(1.0f, 0.0f)),

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
        Vertex(Vector3(-0.5f, -0.5f, 0.5f), Vector3::down, Vector2(0.0f, 1.0f)),
        Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector3::down, Vector2(1.0f, 1.0f)),
        Vertex(Vector3(-0.5f, -0.5f, -0.5f), Vector3::down, Vector2(0.0f, 0.0f)),
        Vertex(Vector3(0.5f, -0.5f, -0.5f), Vector3::down, Vector2(1.0f, 0.0f)),
    };
    const uint32_t NUM_INDICES = 12*3;
    uint32_t indices[NUM_INDICES] = {
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

    setup(vertices, NUM_VERTICES, indices, NUM_INDICES);
}

MeshPrimitives::Quad::Quad() : Mesh()
{
    // Single-sied quad that faces +Z
    const uint32_t NUM_VERTICES = 4;
    Vertex vertices[NUM_VERTICES] = {
        Vertex(Vector3(-0.5f, 0.5f, 0.0f), Vector3::forward, Vector2(0.0f, 1.0f)),
        Vertex(Vector3(0.5f, 0.5f, 0.0f), Vector3::forward, Vector2(1.0f, 1.0f)),
        Vertex(Vector3(-0.5f, -0.5f, 0.0f), Vector3::forward, Vector2(0.0f, 0.0f)),
        Vertex(Vector3(0.5f, -0.5f, 0.0f), Vector3::forward, Vector2(1.0f, 0.0f))
    };
    const uint32_t NUM_INDICES = 2*3;
    uint32_t indices[NUM_INDICES] = {
        0, 1, 3,
        0, 3, 2
    };

    setup(vertices, NUM_VERTICES, indices, NUM_INDICES);
}

MeshPrimitives::Cube* MeshPrimitives::cube = nullptr;
MeshPrimitives::Quad* MeshPrimitives::quad = nullptr;

void MeshPrimitives::Init()
{
    cube = new Cube();
    quad = new Quad();
}
