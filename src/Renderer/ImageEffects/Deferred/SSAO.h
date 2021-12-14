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
    BufferTexture* m_EmissionOcclGBufferCopy;
    Texture2D* m_NoiseTex;
    std::array<BufferTexture*, 2> m_OcclusionBuffers;
};
