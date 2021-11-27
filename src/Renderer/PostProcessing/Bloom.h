#ifndef BLOOM_H
#define BLOOM_H

#include <vector>

#include "../Material.h"
#include "../Texture/Texture.h"
#include "PostProcessing.h"

#include "imgui.h"

class Bloom : public ImageEffect
{
public:
    Bloom();
    ~Bloom();
    void render(BufferTexture* source, BufferTexture* destination) override;
private:
    const float BLUR_SIZE = 2.0f;

    static const size_t BLUR_ITERATIONS = 7;
    static const size_t MAX_BUFFER_COUNT = BLUR_ITERATIONS + 1;

    std::vector<BufferTexture*> m_Buffers;
    Material* m_PrefilterMat;
    Material* m_DownsampleMat;
    Material* m_UpsampleMat;
};

#endif // BLOOM_H
