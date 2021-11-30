#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

#include <iostream>
#include <vector>

#include "../Camera.h"
#include "../FullscreenTriangle.h"
#include "../Material.h"
#include "../Shader.h"
#include "../Texture/Texture.h"


// Forward declaration
class Camera;


class ImageEffect
{
public:
    ImageEffect(const std::string& shaderPath);
    ~ImageEffect();
    virtual void render(BufferTexture* source, BufferTexture* destination);
    void render(BufferTexture* source, BufferTexture* destination, Material* mat);
    void setCamera(Camera* camera);
protected:
    Camera* m_Camera;
    Shader* m_Shader;
    Material* m_Material;
    FullscreenTriangle* m_Triangle;
};

class ImageEffectChain
{
public:
    ImageEffectChain(Camera* camera);
    ~ImageEffectChain();
    void add(ImageEffect* effect);
    void render(BufferTexture* source);
private:
    Camera* m_Camera;
    Material* m_CopyMat;
    FullscreenTriangle* m_Triangle;
    std::array<BufferTexture*, 2> m_ColorBuffers;
    std::vector<ImageEffect*> m_Effects;
};

#endif // POST_PROCESSING_H
