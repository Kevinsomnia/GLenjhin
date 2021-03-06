#pragma once

#include "DeferredEffect.h"

class SSAO : public DeferredEffect
{
public:
    SSAO();
    ~SSAO();
    void lazyInitialize(Camera* camera) override;
    void render() override;
private:
    const size_t DOWNSAMPLE = 1;
    const int BLUR_ITERATIONS = 3;

    Material* m_OcclusionMat;
    Material* m_BlurMat;
    Texture2D* m_NoiseTex;
};
