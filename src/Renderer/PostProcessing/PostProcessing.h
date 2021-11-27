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
    virtual void render(BufferTexture* source, BufferTexture* destination);
protected:
    Shader* m_Shader;
    Material* m_Material;
    FullscreenTriangle* m_Triangle;
};

class ImageEffectChain
{
public:
    ImageEffectChain();
    ~ImageEffectChain();
    void add(ImageEffect* effect);
    void render(BufferTexture* screen);
private:
    std::array<BufferTexture*, 2> m_ColorBuffers;
    std::vector<ImageEffect*> m_Effects;
};

#endif // POST_PROCESSING_H
