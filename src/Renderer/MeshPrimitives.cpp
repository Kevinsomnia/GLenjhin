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
    setGeometry(vertices.data(), vertices.size(), indices.data(), indices.size());
    calculateTangents();
    updateBuffers();
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
    setGeometry(vertices.data(), vertices.size(), indices.data(), indices.size());
    calculateTangents();
    updateBuffers();
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

    for (uint16_t x = 0; x < resolution; x++)
    {
        // Traversing down a column of vertices.
        uint32_t currStripStartVertex = x * resolution;
        uint32_t nextStripStartVertex = currStripStartVertex + resolution;

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

    setGeometry(vertices.data(), vertices.size(), indices, indexCount);
    calculateTangents();
    updateBuffers();
}

MeshPrimitives::Capsule::Capsule(uint16_t resolution) : Mesh()
{
    resolution = std::max(3Ui16, resolution) * 2;

    // Spacing values for vertex positions and UVs.
    float thetaSpacing = PI_FLOAT * 2.0f / resolution;      // Horizontal angle per iteration.
    float phiSpacing = PI_FLOAT / (resolution - 2);         // Vertical angle per iteration.
    Vector2 uvSpacing(1.0f / resolution, 1.0f / (resolution - 1));

    const float CONNECTOR_QUAD_HEIGHT = 0.25f;

    // Compute vertex positions.
    std::vector<Vertex> vertices;

    // Each ring will contain ('resolution' + 1) points (longitude).
    // The starting and ending column will have two different vertices at the same location.
    for (uint16_t i = 0; i <= resolution; i++)
    {
        float theta = i * thetaSpacing;
        float x = sin(theta);
        float z = cos(theta);

        // Form 'resolution' rings for the top hemisphere, top to bottom (latitude).
        float phi = PI_FLOAT * 0.5f;
        for (uint16_t j = 0; j < resolution / 2; j++)
        {
            // Radius of the ring.
            float r = cos(phi) * 0.5f;
            float y = sin(phi) * 0.5f + CONNECTOR_QUAD_HEIGHT;  // equivalent to r * tan(phi)

            float rx = r * x;
            float rz = r * z;

            Vector3 pos = Vector3(rx, y, rz);
            Vector3 normal = Vector3(rx, y - CONNECTOR_QUAD_HEIGHT, rz) * 2.0f;

            float uvX = i * uvSpacing.getX();
            float uvY = j * uvSpacing.getY();

            // We're starting at the top of the sphere, which means we are starting at the top of the texture, so flip Y.
            Vector2 flippedUV = Vector2(uvX, 1.0f - uvY);
            vertices.push_back(Vertex(pos, normal, flippedUV));

            phi -= phiSpacing;
        }

        // Form the bottom hemisphere.
        phi = 0.0f;
        for (uint16_t j = 0; j < resolution / 2; j++)
        {
            // Radius of the ring.
            float r = cos(phi) * 0.5f;
            float y = sin(phi) * 0.5f - CONNECTOR_QUAD_HEIGHT;  // equivalent to r * tan(phi)

            float rx = r * x;
            float rz = r * z;

            Vector3 pos = Vector3(rx, y, rz);
            Vector3 normal = Vector3(rx, y + CONNECTOR_QUAD_HEIGHT, rz) * 2.0f;

            float uvX = i * uvSpacing.getX();
            float uvY = j * uvSpacing.getY();

            // We're starting at the top of the sphere, which means we are starting at the top of the texture, so flip Y.
            Vector2 flippedUV = Vector2(uvX, 1.0f - uvY);
            vertices.push_back(Vertex(pos, normal, flippedUV));

            phi -= phiSpacing;
        }
    }

    // We now have vertices ordered by latitude first (going down column), then moving to next longitude (to the next column).
    // We can calculate number of triangles by computing triangle count for a column strip, and multiplying by the 'resolution' (column count).
    // There are ('resolution'-2) quads, 2 triangles for both poles for each strip, and 2 triangles for connector quad.
    size_t triangleStripCount = 2 * resolution;
    size_t triangleCount = triangleStripCount * resolution;
    size_t indexCount = triangleCount * 3;
    uint32_t* indices = new uint32_t[indexCount];

    size_t i = 0;   // Indices index

    for (uint16_t x = 0; x < resolution; x++)
    {
        // Traversing down a column of vertices.
        uint32_t currStripStartVertex = x * resolution;
        uint32_t nextStripStartVertex = currStripStartVertex + resolution;

        // Handle north pole triangle.
        indices[i++] = currStripStartVertex;
        indices[i++] = nextStripStartVertex + 1;
        indices[i++] = currStripStartVertex + 1;

        // Form quads in between poles.
        for (uint16_t y = 1; y < resolution; y++)
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

    setGeometry(vertices.data(), vertices.size(), indices, indexCount);
    calculateTangents();
    updateBuffers();
}

MeshPrimitives::Cylinder::Cylinder(uint16_t resolution) : Mesh()
{
    resolution = std::max(3Ui16, resolution);

    // Spacing values for vertex positions and UVs.
    float thetaSpacing = PI_FLOAT * 2.0f / resolution;
    float uSpacing = 1.0f / resolution;

    // Compute vertex positions.
    std::vector<Vertex> vertices;

    // Form two disks for the cylinder
    for (uint16_t i = 0; i < 2; i++)
    {
        bool topDisk = i == 0;
        Vector3 diskNormal = topDisk ? Vector3::up : Vector3::down;
        float y = topDisk ? 0.5f : -0.5f;

        vertices.push_back(Vertex(Vector3(0.0f, y, 0.0f), diskNormal, Vector2(0.5f, 0.5f)));

        for (uint16_t j = 0; j <= resolution; j++)
        {
            float theta = j * thetaSpacing;
            float x = 0.5f * sin(theta);
            float z = 0.5f * cos(theta);

            Vector3 position = Vector3(x, y, z);
            Vector3 outNormal = Vector3(x, 0.0f, z) * 2.0f;

            // We need two vertices: one for disk, other for outward facing quad.
            vertices.push_back(Vertex(position, diskNormal, Vector2(x + 0.5f, topDisk ? z + 0.5f : 0.5f - z)));
            vertices.push_back(Vertex(position, outNormal, Vector2(1.0f - (j * uSpacing), topDisk ? 1.0f : 0.0f)));
        }
    }

    // 4 triangles per section, `resolution` number of sections.
    uint32_t triangleCount = 4 * resolution;
    uint32_t indexCount = triangleCount * 3;
    uint32_t* indices = new uint32_t[indexCount];
    size_t i = 0;   // Indices index

    // Form two disks for the cylinder
    uint32_t topDiskCenterIndex = 0;
    uint32_t bottomDiskCenterIndex = 2 * resolution + 3;

    for (uint16_t j = 0; j < resolution; j++)
    {
        uint16_t twoJ = 2 * j;

        uint16_t diskCurr = twoJ + 1;
        uint16_t diskNext = twoJ + 3;
        uint16_t outCurr = twoJ + 2;
        uint16_t outNext = twoJ + 4;

        // Top disk triangle
        indices[i++] = topDiskCenterIndex;
        indices[i++] = topDiskCenterIndex + diskNext;
        indices[i++] = topDiskCenterIndex + diskCurr;

        // Out-facing quad
        // Top triangle.
        indices[i++] = topDiskCenterIndex + outCurr;
        indices[i++] = topDiskCenterIndex + outNext;
        indices[i++] = bottomDiskCenterIndex + outCurr;

        // Bottom triangle.
        indices[i++] = topDiskCenterIndex + outNext;
        indices[i++] = bottomDiskCenterIndex + outNext;
        indices[i++] = bottomDiskCenterIndex + outCurr;

        // Bottom disk triangle
        indices[i++] = bottomDiskCenterIndex;
        indices[i++] = bottomDiskCenterIndex + diskCurr;
        indices[i++] = bottomDiskCenterIndex + diskNext;
    }

    setGeometry(vertices.data(), vertices.size(), indices, indexCount);
    calculateTangents();
    updateBuffers();
}

Mesh* MeshPrimitives::cube = nullptr;
Mesh* MeshPrimitives::quad = nullptr;
Mesh* MeshPrimitives::sphere = nullptr;
Mesh* MeshPrimitives::capsule = nullptr;
Mesh* MeshPrimitives::cylinder = nullptr;

void MeshPrimitives::Init()
{
    cube = new Cube();
    quad = new Quad();
    sphere = new Sphere(32);
    capsule = new Capsule(24);
    cylinder = new Cylinder(32);
}
