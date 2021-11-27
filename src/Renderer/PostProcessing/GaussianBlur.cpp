#include "GaussianBlur.h"

GaussianBlur::GaussianBlur() : ImageEffect("res\\shaders\\PostProcessing\\GaussianBlur.shader")
{
    const int SCR_WIDTH = 1600;
    const int SCR_HEIGHT = 900;
    const int DOWNSAMPLE = 1;

    const int WIDTH = SCR_WIDTH >> DOWNSAMPLE;
    const int HEIGHT = SCR_HEIGHT >> DOWNSAMPLE;

    m_DownsampleBuffer = new BufferTexture(WIDTH, HEIGHT, /*depth=*/ 0, TextureFormat::RGB24);
}

GaussianBlur::~GaussianBlur()
{
    if (m_DownsampleBuffer)
        delete m_DownsampleBuffer;
}

void GaussianBlur::render(BufferTexture* source, BufferTexture* destination)
{
    // Downsample
    glViewport(0, 0, m_DownsampleBuffer->width(), m_DownsampleBuffer->height());
    ImageEffect::render(source, m_DownsampleBuffer);

    // TODO: two-pass blur
    glViewport(0, 0, 1600, 900);
    ImageEffect::render(m_DownsampleBuffer, destination);
}
