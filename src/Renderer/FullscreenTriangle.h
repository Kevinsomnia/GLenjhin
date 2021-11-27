#ifndef FULLSCREEN_TRIANGLE_H
#define FULLSCREEN_TRIANGLE_H

#include <iostream>
#include "Material.h"

using std::cout;
using std::endl;

class FullscreenTriangle
{
public:
    FullscreenTriangle(Material* mat);
    ~FullscreenTriangle();
    void setMaterial(Material* mat);
    void draw() const;
private:
    static uint32_t VAO_ID;

    Material* m_Material;
};

#endif // FULLSCREEN_TRIANGLE_H
