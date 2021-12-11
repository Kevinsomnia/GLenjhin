#ifndef IMAGE_EFFECT_CHAIN_H
#define IMAGE_EFFECT_CHAIN_H

#include <iostream>
#include <vector>

#include "../../Math/Math.h"
#include "../Camera.h"
#include "../FullscreenTriangle.h"
#include "../Material.h"
#include "../Texture/Texture.h"
#include "ImageEffect.h"


// Forward declaration
class Camera;
class ImageEffect;


enum class ImageEffectChainType
{
    PostProcess,
    Deferred
};

class ImageEffectChain
{
public:
    ImageEffectChain(Camera* camera, ImageEffectChainType type);
    ~ImageEffectChain();
    void add(ImageEffect* effect);
    void render();
private:
    ImageEffectChainType m_Type;
    Camera* m_Camera;
    Material* m_CopyMat;
    FullscreenTriangle* m_Triangle;
    std::array<BufferTexture*, 2> m_ColorBuffers;
    size_t m_NumColorBuffers;  // Valid values: 0, 1, or 2. Represents how many m_ColorBuffers are in use.
    std::vector<ImageEffect*> m_Effects;
};

#endif // IMAGE_EFFECT_CHAIN_H
