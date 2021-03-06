#pragma once

#include "../../Camera.h"
#include "../../Texture/BufferTexture.h"


// Forward declaration
class Camera;


class DeferredEffect
{
public:
    DeferredEffect(const std::string& shaderPath);
    ~DeferredEffect();
    virtual void lazyInitialize(Camera* camera);
    virtual void render();
    void render(BufferTexture* destination, Material* mat) const;
    void render(BufferTexture* source, BufferTexture* destination, Material* mat) const;
    void render(Texture2D* source, BufferTexture* destination, Material* mat) const;
protected:
    bool m_Initialized;
    Camera* m_Camera;
    Material* m_Material;
    Material* m_CopyMat;
};
