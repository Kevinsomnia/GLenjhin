#ifndef FULLSCREEN_TRIANGLE_H
#define FULLSCREEN_TRIANGLE_H

#include "Material.h"

using std::cout;
using std::endl;

class FullscreenTriangle
{
public:
    FullscreenTriangle(Material* mat, bool depthTest = false);
    ~FullscreenTriangle();
    void setMaterial(Material* mat);
    void draw() const;
private:
    static uint32_t VAO_ID;

    Material* m_Material;
    bool m_DepthTest;
};

#endif // FULLSCREEN_TRIANGLE_H
