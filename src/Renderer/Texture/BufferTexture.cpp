#include "BufferTexture.h"

BufferTexture::BufferTexture(uint32_t width, uint32_t height, TextureFormat colorFormat, TextureFormat depthFormat) : Texture()
{
    assert(colorFormat != TextureFormat::None || depthFormat != TextureFormat::None);
    m_Width = width;
    m_Height = height;

    glGenFramebuffers(1, &m_TextureID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_TextureID);

    if (colorFormat != TextureFormat::None)
    {
        m_ColorTexture = new Texture2D(width, height, colorFormat, /*readable=*/ false, /*sRGB=*/ false);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture->id(), 0);
    }

    if (depthFormat != TextureFormat::None)
    {
        assert(depthFormat == TextureFormat::Depth16 || depthFormat == TextureFormat::Depth24 || depthFormat == TextureFormat::Depth32);

        // Depth texture should always have point filtering.
        m_DepthTexture = new Texture2D(width, height, depthFormat, /*readable=*/ false, /*sRGB=*/ false);
        m_DepthTexture->setFilterMode(TextureFilterMode::Point);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture->id(), 0);

        if (colorFormat == TextureFormat::None)
        {
            // Tell OpenGL we're rendering to depth only.
            glReadBuffer(GL_NONE);
            glDrawBuffer(GL_NONE);
        }
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

void BufferTexture::bind() const
{
    glViewport(0, 0, m_Width, m_Height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_TextureID);
    GlobalStats::AddFramebufferBindCall();
}

void BufferTexture::bind(uint32_t slotIndex) const
{
    bind();
}

void BufferTexture::setFilterMode(TextureFilterMode filterMode)
{
    m_FilterMode = filterMode;
    if (m_ColorTexture)
        m_ColorTexture->setFilterMode(filterMode);
    // Don't change depth texture filter mode from point filtering.
}

void BufferTexture::setWrapMode(TextureWrapMode wrapMode)
{
    m_WrapMode = wrapMode;
    if (m_ColorTexture)
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
