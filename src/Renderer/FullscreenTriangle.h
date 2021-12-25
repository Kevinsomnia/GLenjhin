#pragma once

#include "Material.h"

using std::cout;
using std::endl;

class FullscreenTriangle
{
public:
    FullscreenTriangle();
    static void Init();
    static void Draw(const Material& mat, bool depthTest);

private:
    static uint32_t VAO_ID;
    static FullscreenTriangle* instance;
};
