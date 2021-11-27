#ifndef GAUSSIAN_BLUR_H
#define GAUSSIAN_BLUR_H

#include "PostProcessing.h"

class GaussianBlur : public ImageEffect
{
public:
    GaussianBlur();
    ~GaussianBlur();
    void render(BufferTexture* source, BufferTexture* destination) override;
private:
    BufferTexture* m_DownsampleBuffer;
};

#endif // GAUSSIAN_BLUR_H
