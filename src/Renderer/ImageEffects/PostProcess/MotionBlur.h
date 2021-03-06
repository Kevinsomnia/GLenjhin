#pragma once

#include "PostProcessEffect.h"

class MotionBlur : public PostProcessEffect
{
public:
    MotionBlur();
    ~MotionBlur();
    void lazyInitialize(Camera* camera) override;
    void render(BufferTexture* source, BufferTexture* destination) override;

private:
    static const int SAMPLE_COUNT = 8;
    const float MAX_BLUR_RADIUS = 0.1f; // Percentage of screen height
    const float BLUR_STRENGTH = 0.35f;

    uint32_t m_TileSize;
    Material* m_PackVelocityDepthMat;
    Material* m_MaxVelocityDenormalizeMat;
    Material* m_MaxVelocityMat;
    Material* m_MaxTileVelocityMat;
    Material* m_MaxNeighborVelocityMat;
    Material* m_ReconstructionMat;
    std::array<BufferTexture*, 3> m_MaxVelocityTex;
};
