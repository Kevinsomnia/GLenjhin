#ifndef SKYBOX_H
#define SKYBOX_H

#include <iostream>
#include "Camera.h"
#include "FullscreenTriangle.h"

using std::cout;
using std::endl;

class Skybox
{
public:
    Skybox(const std::string& texturePath);
    ~Skybox();
    void draw(const Camera& vp);
private:
    Texture2D* m_Texture;
    Material* m_Material;
    FullscreenTriangle* m_Triangle;
};

#endif // SKYBOX_H
