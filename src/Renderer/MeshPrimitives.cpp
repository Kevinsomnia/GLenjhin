#include "MeshPrimitives.h"

MeshPrimitives::Cube::Cube() : Mesh()
{
	const uint32_t NUM_VERTICES = 8;
	Vertex vertices[NUM_VERTICES] = {
		Vertex(Vector3(-0.5f, 0.5f, 0.5f)),
		Vertex(Vector3(0.5f, 0.5f, 0.5f)),
		Vertex(Vector3(-0.5f, -0.5f, 0.5f)),
		Vertex(Vector3(0.5f, -0.5f, 0.5f)),
		Vertex(Vector3(-0.5f, 0.5f, -0.5f)),
		Vertex(Vector3(0.5f, 0.5f, -0.5f)),
		Vertex(Vector3(-0.5f, -0.5f, -0.5f)),
		Vertex(Vector3(0.5f, -0.5f, -0.5f))
	};
	const uint32_t NUM_INDICES = 12*3;
	uint32_t indices[NUM_INDICES] = {
		// -Z
		4, 7, 5,
		4, 6, 7,
		// +Z
		0, 1, 3,
		0, 3, 2,
		// -X
		2, 4, 0,
		2, 6, 4,
		// +X
		5, 3, 1,
		5, 7, 3,
		// -Y
		7, 2, 3,
		7, 6, 2,
		// +Y
		0, 5, 1,
		0, 4, 5
	};

    setup(vertices, NUM_VERTICES, indices, NUM_INDICES);
}

MeshPrimitives::Cube* MeshPrimitives::cube = nullptr;

void MeshPrimitives::Init()
{
    cube = new Cube();
}
