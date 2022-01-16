#pragma once

#include <vector>

#include "PostProcessEffect.h"

class Bloom : public PostProcessEffect
{
public:
    Bloom();
    ~Bloom();
    void render(BufferTexture* source, BufferTexture* destination) override;
private:
    const float BLUR_SIZE = 1.0f;

    static const size_t BLUR_ITERATIONS = 7;
    static const size_t MAX_BUFFER_COUNT = BLUR_ITERATIONS + 1;

    std::array<BufferTexture*, MAX_BUFFER_COUNT> m_Buffers;
    size_t m_BufferCount;
    Material* m_PrefilterMat;
    Material* m_DownsampleMat;
    Material* m_UpsampleMat;
};
