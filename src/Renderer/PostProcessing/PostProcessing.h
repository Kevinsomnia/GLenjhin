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
    void setContext(Camera* camera, BufferTexture* screen);
protected:
    Camera* m_Camera;
    BufferTexture* m_ScreenBuffer;
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
    void render(BufferTexture* screen);
private:
    Camera* m_Camera;
    std::array<BufferTexture*, 2> m_ColorBuffers;
    std::vector<ImageEffect*> m_Effects;
};

#endif // POST_PROCESSING_H
