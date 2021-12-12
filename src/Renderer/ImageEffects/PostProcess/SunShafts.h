#ifndef SUN_SHAFTS_H
#define SUN_SHAFTS_H

#include "PostProcessEffect.h"

class SunShafts : public PostProcessEffect
{
public:
    SunShafts();
    ~SunShafts();
    void lazyInitialize(Camera* camera) override;
    void render(BufferTexture* source, BufferTexture* destination) override;
    void setSunTransform(Transform* sun) { m_SunTransform = sun; }
private:
    const float BLUR_RADIUS = 12.0f;
    static const size_t DOWNSAMPLE = 1;
    static const size_t BLUR_ITERATIONS = 2;

    Transform* m_SunTransform;
    Material* m_DepthFilterMat;
    Material* m_RadialBlurMat;
    std::array<BufferTexture*, 2> m_Buffers;
};

#endif // SUN_SHAFTS_H
