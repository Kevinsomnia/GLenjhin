#pragma once

#include <vector>

#include "../../../Math/Math.h"
#include "../../Camera.h"
#include "../../FullscreenTriangle.h"
#include "../../Material.h"
#include "DeferredEffect.h"


// Forward declaration
class Camera;
class DeferredEffect;


class DeferredEffectChain
{
public:
    DeferredEffectChain(Camera* camera);
    ~DeferredEffectChain();
    void add(DeferredEffect* effect);
    void render();
private:
    Camera* m_Camera;
    Material* m_CopyMat;
    std::vector<DeferredEffect*> m_Effects;
};
