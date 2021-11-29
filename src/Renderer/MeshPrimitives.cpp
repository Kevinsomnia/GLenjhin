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

MeshPrimitives::Sphere::Sphere(uint16_t resolution) : Mesh()
{
    resolution = std::max(3Ui16, resolution);

    // Spacing values for vertex positions and UVs.
    float thetaSpacing = PI_FLOAT * 2.0f / resolution;  // Horizontal angle per iteration.
    float phiSpacing = PI_FLOAT / (resolution - 1);     // Vertical angle per iteration.
    Vector2 uvSpacing(1.0f / resolution, 1.0f / (resolution - 1));

    // Compute vertex positions.
    std::vector<Vertex> vertices;

    // Each ring will contain ('resolution' + 1) points (longitude).
    // The starting and ending column will have two different vertices at the same location.
    for (uint16_t i = 0; i <= resolution; i++)
    {
        float theta = i * thetaSpacing;
        float x = sin(theta);
        float z = cos(theta);

        // Form 'resolution' rings around sphere, top to bottom (latitude).
        float phi = PI_FLOAT * 0.5f;
        for (uint16_t j = 0; j < resolution; j++)
        {
            // Radius of the ring.
            float r = cos(phi) * 0.5f;
            float y = sin(phi) * 0.5f;  // equivalent to r * tan(phi)

            float rx = r * x;
            float rz = r * z;

            Vector3 pos = Vector3(rx, y, rz);
            // Position is already normalized, but length is 0.5 so we just double the vector to correct it.
            Vector3 normal = pos * 2.0f;

            float uvX = i * uvSpacing.getX();
            float uvY = j * uvSpacing.getY();

            // For the poles, those vertices will be the midpoint between two columns in the UV.
            // See this image for reference: https://upload.wikimedia.org/wikipedia/commons/0/04/UVMapping.png
            // Specifically look at the top and bottom row on the UV map.
            if (j == 0 || j == resolution - 1)
                uvX += uvSpacing.getX() * 0.5f;

            // We're starting at the top of the sphere, which means we are starting at the top of the texture, so flip Y.
            // Also, flip X since the triangles are facing outwards.
            Vector2 flippedUV = Vector2(1.0f - uvX, 1.0f - uvY);
            vertices.push_back(Vertex(pos, normal, flippedUV));

            phi -= phiSpacing;
        }
    }

    // We now have vertices ordered by latitude first (going down column), then moving to next longitude (to the next column).
    // We can calculate number of triangles by computing triangle count for a column strip, and multiplying by the 'resolution' (column count).
    // There are ('resolution'-3) quads, and 2 triangles for both poles for each strip.
    size_t triangleStripCount = 2 * (resolution - 2);
    size_t triangleCount = triangleStripCount * resolution;
    size_t indexCount = triangleCount * 3;
    uint32_t* indices = new uint32_t[indexCount];

    size_t i = 0;   // Indices index
    const size_t northPoleVertexIndex = 0;
    size_t southPoleVertexIndex = vertices.size() - 1;

    for (uint16_t x = 0; x < resolution; x++)
    {
        // Traversing down a column of vertices.
        size_t currStripStartVertex = x * resolution;
        size_t nextStripStartVertex = currStripStartVertex + resolution;

        // Handle north pole triangle.
        indices[i++] = currStripStartVertex;
        indices[i++] = nextStripStartVertex + 1;
        indices[i++] = currStripStartVertex + 1;

        // Form quads in between poles.
        for (uint16_t y = 1; y < resolution - 2; y++)
        {
            // Top triangle.
            indices[i++] = currStripStartVertex + y;
            indices[i++] = nextStripStartVertex + y;
            indices[i++] = nextStripStartVertex + y + 1;

            // Bottom triangle.
            indices[i++] = currStripStartVertex + y;
            indices[i++] = nextStripStartVertex + y + 1;
            indices[i++] = currStripStartVertex + y + 1;
        }

        // Handle south pole triangle.
        indices[i++] = nextStripStartVertex - 2;
        indices[i++] = nextStripStartVertex + resolution - 2;
        indices[i++] = nextStripStartVertex - 1;
    }

    setup(vertices.data(), vertices.size(), indices, indexCount);
}

Mesh* MeshPrimitives::cube = nullptr;
Mesh* MeshPrimitives::quad = nullptr;
Mesh* MeshPrimitives::sphere = nullptr;

void MeshPrimitives::Init()
{
    cube = new Cube();
    quad = new Quad();
    sphere = new Sphere(32);
}
