#pragma once

#include "PostProcessEffect.h"

class GaussianBlur : public PostProcessEffect
{
public:
    GaussianBlur();
    ~GaussianBlur();
    void render(BufferTexture* source, BufferTexture* destination) override;
private:
    const float BLUR_RADIUS = 8.0f;
    const float REFERENCE_HEIGHT = 1080.0f;
    const int BLUR_ITERATIONS = 4;
    const int DOWNSAMPLE = 1;

    Material* m_DownsampleMat;
    Material* m_BlurMat;
};
