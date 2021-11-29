#ifndef SKYBOX_H
#define SKYBOX_H

#include <iostream>
#include "../Math/Matrix4x4.h"
#include "FullscreenTriangle.h"

using std::cout;
using std::endl;

class Skybox
{
public:
    Skybox(const std::string& texturePath);
    ~Skybox();
    void draw(const Matrix4x4& vp);
private:
    Texture2D* m_Texture;
    Material* m_Material;
    FullscreenTriangle* m_Triangle;
};

#endif // SKYBOX_H
