#ifndef POST_PROCESS_EFFECT_CHAIN_H
#define POST_PROCESS_EFFECT_CHAIN_H

#include <iostream>
#include <vector>

#include "../../../Math/Math.h"
#include "../../Camera.h"
#include "../../FullscreenTriangle.h"
#include "../../Material.h"
#include "../../Texture/Texture.h"
#include "PostProcessEffect.h"


// Forward declaration
class Camera;
class PostProcessEffect;


class PostProcessEffectChain
{
public:
    PostProcessEffectChain(Camera* camera);
    ~PostProcessEffectChain();
    void add(PostProcessEffect* effect);
    void render();
private:
    Camera* m_Camera;
    Material* m_CopyMat;
    FullscreenTriangle* m_Triangle;
    std::array<BufferTexture*, 2> m_ColorBuffers;
    size_t m_NumColorBuffers;  // Valid values: 0, 1, or 2. Represents how many m_ColorBuffers are in use.
    std::vector<PostProcessEffect*> m_Effects;
};

#endif // POST_PROCESS_EFFECT_CHAIN_H
