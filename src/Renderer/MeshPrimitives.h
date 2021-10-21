#ifndef MESHPRIMITIVES_H
#define MESHPRIMITIVES_H

#include <array>
#include <vector>

#include "../Math/Math.h"
#include "Mesh.h"

class MeshPrimitives
{
public:
    class Cube: public Mesh
    {
    public:
        Cube();
    };

    class Quad: public Mesh
    {
    public:
        Quad();
    };

    class Sphere: public Mesh
    {
    public:
        Sphere() = delete;
        Sphere(uint16_t resolution);
    };

    static Mesh* cube;
    static Mesh* quad;
    static Mesh* sphere;
    static void Init();
};

#endif
