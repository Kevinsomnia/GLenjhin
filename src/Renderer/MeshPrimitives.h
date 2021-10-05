#ifndef MESHPRIMITIVES_H
#define MESHPRIMITIVES_H

#include "Mesh.h"

class MeshPrimitives
{
public:
    class Cube: public Mesh
    {
    public:
        Cube();
    };

    static Cube* cube;
    static void Init();
};

#endif
