#include "MeshPrimitives.h"

MeshPrimitives::Cube::Cube() : Mesh()
{
	const uint32_t NUM_VERTICES = 24;
	Vertex vertices[NUM_VERTICES] = {
		// +Z
		Vertex(Vector3(-0.5f, 0.5f, 0.5f), Vector3::forward),
		Vertex(Vector3(0.5f, 0.5f, 0.5f), Vector3::forward),
		Vertex(Vector3(-0.5f, -0.5f, 0.5f), Vector3::forward),
		Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector3::forward),

		// -Z
		Vertex(Vector3(-0.5f, 0.5f, -0.5f), Vector3::back),
		Vertex(Vector3(0.5f, 0.5f, -0.5f), Vector3::back),
		Vertex(Vector3(-0.5f, -0.5f, -0.5f), Vector3::back),
		Vertex(Vector3(0.5f, -0.5f, -0.5f), Vector3::back),

		// +X
		Vertex(Vector3(0.5f, 0.5f, 0.5f), Vector3::right),
		Vertex(Vector3(0.5f, 0.5f, -0.5f), Vector3::right),
		Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector3::right),
		Vertex(Vector3(0.5f, -0.5f, -0.5f), Vector3::right),

		// -X
		Vertex(Vector3(-0.5f, 0.5f, 0.5f), Vector3::left),
		Vertex(Vector3(-0.5f, 0.5f, -0.5f), Vector3::left),
		Vertex(Vector3(-0.5f, -0.5f, 0.5f), Vector3::left),
		Vertex(Vector3(-0.5f, -0.5f, -0.5f), Vector3::left),

		// +Y
		Vertex(Vector3(-0.5f, 0.5f, 0.5f), Vector3::up),
		Vertex(Vector3(0.5f, 0.5f, 0.5f), Vector3::up),
		Vertex(Vector3(-0.5f, 0.5f, -0.5f), Vector3::up),
		Vertex(Vector3(0.5f, 0.5f, -0.5f), Vector3::up),

		// -Y
		Vertex(Vector3(-0.5f, -0.5f, 0.5f), Vector3::down),
		Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector3::down),
		Vertex(Vector3(-0.5f, -0.5f, -0.5f), Vector3::down),
		Vertex(Vector3(0.5f, -0.5f, -0.5f), Vector3::down),
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

MeshPrimitives::Cube* MeshPrimitives::cube = nullptr;

void MeshPrimitives::Init()
{
    cube = new Cube();
}
