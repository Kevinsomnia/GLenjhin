#include "Texture.h"

BufferTexture::BufferTexture(int width, int height, int depth, TextureFormat colorFormat) : Texture()
{
    m_Width = width;
    m_Height = height;

    glGenFramebuffers(1, &m_TextureID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_TextureID);

    m_ColorTexture = new Texture2D(width, height, colorFormat);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture->id(), 0);

    if (depth != 0)
    {
        TextureFormat depthMode = TextureFormat::Depth16;

        if (depth == 24)
        {
            depthMode = TextureFormat::Depth24;
        }
        else if (depth == 32)
        {
            depthMode = TextureFormat::Depth32;
        }

        // Depth texture should always have point filtering. Might change in the future.
        m_DepthTexture = new Texture2D(width, height, depthMode);
        m_DepthTexture->setFilterMode(TextureFilterMode::Point);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture->id(), 0);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        // Finish setup
        setFilterMode(TextureFilterMode::Bilinear);
        setWrapMode(TextureWrapMode::Clamp);
    }
    else
    {
        cerr << "BufferTexture failed to initialize. This graphics device probably doesn't support this texture format: " << static_cast<uint32_t>(colorFormat) << endl;
        internalDispose();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
}

BufferTexture::~BufferTexture()
{
    internalDispose();
}

Texture2D* BufferTexture::colorTexture() const
{
    return m_ColorTexture;
}

Texture2D* BufferTexture::depthTexture() const
{
    return m_DepthTexture;
}

void BufferTexture::bind(uint32_t slotIndex) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_TextureID);
}

void BufferTexture::setFilterMode(TextureFilterMode filterMode)
{
    m_FilterMode = filterMode;
    m_ColorTexture->setFilterMode(filterMode);
    // Don't change depth texture filter mode from point filtering.
}

void BufferTexture::setWrapMode(TextureWrapMode wrapMode)
{
    m_WrapMode = wrapMode;
    m_ColorTexture->setWrapMode(wrapMode);

    if (m_DepthTexture)
        m_DepthTexture->setWrapMode(wrapMode);
}

void BufferTexture::internalDispose()
{
    glDeleteFramebuffers(1, &m_TextureID);
    m_TextureID = NULL;

    if (m_ColorTexture)
        delete m_ColorTexture;

    if (m_DepthTexture)
        delete m_DepthTexture;
}
