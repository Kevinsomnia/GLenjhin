#ifndef DEFERRED_EFFECT_H
#define DEFERRED_EFFECT_H

#include "../../Camera.h"
#include "../../FullscreenTriangle.h"
#include "../../Material.h"
#include "../../Shader.h"
#include "../../Texture/Texture.h"


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
protected:
    bool m_Initialized;
    Camera* m_Camera;
    Shader* m_Shader;
    Material* m_Material;
    Material* m_CopyMat;
    FullscreenTriangle* m_Triangle;
};

#endif // DEFERRED_EFFECT_H
