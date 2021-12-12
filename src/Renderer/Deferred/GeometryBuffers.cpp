#include "GeometryBuffers.h"

GeometryBuffers::GeometryBuffers(uint32_t width, uint32_t height, uint8_t depth) : m_Width(width), m_Height(height)
{
    glGenFramebuffers(1, &m_FboID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);

    m_PositionGBuffer = new Texture2D(width, height, TextureFormat::RGBAFloat, /*readable=*/ false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PositionGBuffer->id(), 0);
    m_NormalSmoothGBuffer = new Texture2D(width, height, TextureFormat::RGBAFloat, /*readable=*/ false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_NormalSmoothGBuffer->id(), 0);
    m_AlbedoMetalGBuffer = new Texture2D(width, height, TextureFormat::RGBA32, /*readable=*/ false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_AlbedoMetalGBuffer->id(), 0);
    m_EmissionGBuffer = new Texture2D(width, height, TextureFormat::RGBAHalf, /*readable=*/ false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_EmissionGBuffer->id(), 0);

    const GLuint ATTACHMENT_COUNT = 4;
    GLenum colorAttachments[ATTACHMENT_COUNT] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(ATTACHMENT_COUNT, colorAttachments);

    if (depth != 0)
    {
        // Depth texture should always have point filtering. Might change in the future.
        m_DepthTexture = new Texture2D(width, height, GetDepthTextureFormat(depth), /*readable=*/ false);
        m_DepthTexture->setFilterMode(TextureFilterMode::Point);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture->id(), 0);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        // Finish setup
        m_PositionGBuffer->setFilterMode(TextureFilterMode::Point);
        m_NormalSmoothGBuffer->setFilterMode(TextureFilterMode::Point);
        m_AlbedoMetalGBuffer->setFilterMode(TextureFilterMode::Point);
        m_EmissionGBuffer->setFilterMode(TextureFilterMode::Point);

        m_PositionGBuffer->setWrapMode(TextureWrapMode::Clamp);
        m_NormalSmoothGBuffer->setWrapMode(TextureWrapMode::Clamp);
        m_AlbedoMetalGBuffer->setWrapMode(TextureWrapMode::Clamp);
        m_EmissionGBuffer->setWrapMode(TextureWrapMode::Clamp);
    }
    else
    {
        cerr << "GeometryBuffer failed to initialize. This graphics device probably doesn't support deferred rendering." << endl;
        internalDispose();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
}

GeometryBuffers::~GeometryBuffers()
{
    internalDispose();
}

void GeometryBuffers::bind() const
{
    glViewport(0, 0, m_Width, m_Height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
}

void GeometryBuffers::setGBufferTextures(Material& mat) const
{
    mat.setTexture("u_Position", m_PositionGBuffer);
    mat.setTexture("u_NormalSmooth", m_NormalSmoothGBuffer);
    mat.setTexture("u_AlbedoMetal", m_AlbedoMetalGBuffer);
    mat.setTexture("u_Emission", m_EmissionGBuffer);
}

void GeometryBuffers::internalDispose()
{
    glDeleteFramebuffers(1, &m_FboID);
    m_FboID = NULL;

    delete m_PositionGBuffer;
    delete m_NormalSmoothGBuffer;
    delete m_AlbedoMetalGBuffer;
    delete m_EmissionGBuffer;

    if (m_DepthTexture)
        delete m_DepthTexture;
}
