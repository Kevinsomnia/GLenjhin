#pragma once

#include <vector>

#include "../../../Math/Math.h"
#include "../../Camera.h"
#include "../../FullscreenTriangle.h"
#include "../../Material.h"
#include "../../Texture/BufferTexturePool.h"
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
    std::array<BufferTexture*, 2> m_ColorBuffers;
    // Valid values: 0, 1, or 2. Represents how many m_ColorBuffers are in use while rendering.
    size_t m_NumColorBuffers;
    std::vector<PostProcessEffect*> m_Effects;
};
