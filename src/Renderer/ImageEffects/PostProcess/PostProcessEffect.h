#pragma once

#include "../../Camera.h"
#include "../../FullscreenTriangle.h"
#include "../../Material.h"
#include "../../Shader.h"
#include "../../Texture/BufferTexture.h"


// Forward declaration
class Camera;


class PostProcessEffect
{
public:
    PostProcessEffect(const std::string& shaderPath);
    ~PostProcessEffect();
    virtual void lazyInitialize(Camera* camera);
    virtual void render(BufferTexture* source, BufferTexture* destination);
    void render(BufferTexture* destination, Material* mat) const;
    void render(BufferTexture* source, BufferTexture* destination, Material* mat) const;
protected:
    bool m_Initialized;
    Camera* m_Camera;
    Shader* m_Shader;
    Material* m_Material;
    FullscreenTriangle* m_Triangle;
};
