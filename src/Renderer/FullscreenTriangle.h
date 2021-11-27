#ifndef FULLSCREEN_TRIANGLE_H
#define FULLSCREEN_TRIANGLE_H

#include <iostream>
#include "Material.h"

using std::cout;
using std::endl;

class FullscreenTriangle
{
public:
    FullscreenTriangle(Material* material);
    ~FullscreenTriangle();
    void draw();
private:
    static uint32_t VAO_ID;

    Material* m_Material;
};

#endif // FULLSCREEN_TRIANGLE_H
