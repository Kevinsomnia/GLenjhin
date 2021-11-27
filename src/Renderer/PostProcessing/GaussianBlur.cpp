#include "GaussianBlur.h"

GaussianBlur::GaussianBlur() : ImageEffect("res\\shaders\\PostProcessing\\GaussianBlur.shader")
{
    const int SCR_WIDTH = 1600;
    const int SCR_HEIGHT = 900;
    const int DOWNSAMPLE = 1;

    const int WIDTH = SCR_WIDTH >> DOWNSAMPLE;
    const int HEIGHT = SCR_HEIGHT >> DOWNSAMPLE;

    glGenFramebuffers(1, &m_DownsampleFboID);
    m_DownsampleBuffer = new BufferTexture(WIDTH, HEIGHT, TextureFormat::RGB24);
    glBindFramebuffer(GL_FRAMEBUFFER, m_DownsampleFboID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_DownsampleBuffer->id(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
}

GaussianBlur::~GaussianBlur()
{
    if (m_DownsampleBuffer)
        delete m_DownsampleBuffer;

    glDeleteFramebuffers(1, &m_DownsampleFboID);
    m_DownsampleFboID = NULL;
}

void GaussianBlur::render(BufferTexture* readBufferTex, uint32_t writeBufferID)
{
    // Downsample
    glViewport(0, 0, m_DownsampleBuffer->width(), m_DownsampleBuffer->height());
    ImageEffect::render(readBufferTex, m_DownsampleFboID);

    // TODO: two-pass blur
    glViewport(0, 0, 1600, 900);
    ImageEffect::render(m_DownsampleBuffer, writeBufferID);
}
