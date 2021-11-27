#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

#include <iostream>
#include <vector>

#include "../FullscreenTriangle.h"
#include "../Material.h"
#include "../Shader.h"
#include "../Texture/Texture.h"

class ImageEffect
{
public:
    ImageEffect(const std::string& shaderPath);
    ~ImageEffect();
    virtual void render(Texture* src);
private:
    Shader* m_Shader;
    Material* m_Material;
    FullscreenTriangle* m_Triangle;
};

#endif // POST_PROCESSING_H
