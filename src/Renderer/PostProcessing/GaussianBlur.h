#ifndef GAUSSIAN_BLUR_H
#define GAUSSIAN_BLUR_H

#include "PostProcessing.h"

class GaussianBlur : public ImageEffect
{
public:
    GaussianBlur();
    ~GaussianBlur();
    void render(Texture* readBufferTex, uint32_t writeBufferID) override;
private:
    uint32_t m_DownsampleFboID;
    BufferTexture* m_DownsampleBuffer;
};

#endif // GAUSSIAN_BLUR_H
