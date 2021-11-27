#include "Texture.h"

BufferTexture::BufferTexture(int width, int height, int depth, TextureFormat colorFormat) : Texture()
{
    m_Width = width;
    m_Height = height;
    m_ColorTexture = nullptr;
    m_DepthTextureID = NULL;

    glGenFramebuffers(1, &m_TextureID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_TextureID);

    m_ColorTexture = new Texture2D(width, height, colorFormat);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture->id(), 0);

    if (depth != 0)
    {
        GLint depthMode = GL_DEPTH_COMPONENT16;

        if (depth == 24)
        {
            depthMode = GL_DEPTH_COMPONENT24;
        }
        else if (depth == 32)
        {
            depthMode = GL_DEPTH_COMPONENT32;
        }

        glGenRenderbuffers(1, &m_DepthTextureID);
        glBindRenderbuffer(GL_RENDERBUFFER, m_DepthTextureID);
        glRenderbufferStorage(GL_RENDERBUFFER, depthMode, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthTextureID);
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

void BufferTexture::bind(uint32_t slotIndex) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_TextureID);
}

void BufferTexture::setFilterMode(TextureFilterMode filterMode)
{
    m_FilterMode = filterMode;
    m_ColorTexture->setFilterMode(filterMode);
    // TODO: set depth texture filter mode if that becomes a Texture2D.
}

void BufferTexture::setWrapMode(TextureWrapMode wrapMode)
{
    m_WrapMode = wrapMode;
    m_ColorTexture->setWrapMode(wrapMode);
    // TODO: set depth texture filter mode if that becomes a Texture2D.
}

void BufferTexture::internalDispose()
{
    glDeleteFramebuffers(1, &m_TextureID);
    m_TextureID = NULL;

    if (m_ColorTexture)
    {
        delete m_ColorTexture;
        m_ColorTexture = nullptr;
    }

    if (m_DepthTextureID != NULL)
    {
        glDeleteFramebuffers(1, &m_DepthTextureID);
        m_DepthTextureID = NULL;
    }
}
