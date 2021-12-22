#pragma once

#include "Material.h"

using std::cout;
using std::endl;

class FullscreenTriangle
{
public:
    FullscreenTriangle(Material* mat, bool depthTest = false);
    ~FullscreenTriangle();
    void setMaterial(Material* mat);
    void setDepthTest(bool depthTest);
    void draw() const;
private:
    static uint32_t VAO_ID;

    Material* m_Material;
    bool m_DepthTest;
};
