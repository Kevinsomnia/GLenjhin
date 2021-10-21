#ifndef MESHPRIMITIVES_H
#define MESHPRIMITIVES_H

#include <array>

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

    static Cube* cube;
    static Quad* quad;
    static void Init();
};

#endif
