#include "GeometryBuffers.h"

GeometryBuffers::GeometryBuffers(int width, int height)
{
    glGenFramebuffers(1, &m_FboID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);

    m_PositionGBuffer = new Texture2D(width, height, TextureFormat::RGBAFloat);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PositionGBuffer->id(), 0);
    m_NormalGBuffer = new Texture2D(width, height, TextureFormat::RGBAHalf);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_NormalGBuffer->id(), 0);
    m_AlbedoSpecGBuffer = new Texture2D(width, height, TextureFormat::RGBA32);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_AlbedoSpecGBuffer->id(), 0);

    std::array<GLenum, 3> colorAttachments = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(colorAttachments.size(), colorAttachments.data());

    // Depth texture should always have point filtering. Might change in the future.
    m_DepthTexture = new Texture2D(width, height, TextureFormat::Depth32);
    m_DepthTexture->setFilterMode(TextureFilterMode::Point);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture->id(), 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        // Finish setup
        m_PositionGBuffer->setFilterMode(TextureFilterMode::Point);
        m_NormalGBuffer->setFilterMode(TextureFilterMode::Point);
        m_AlbedoSpecGBuffer->setFilterMode(TextureFilterMode::Point);
        m_PositionGBuffer->setWrapMode(TextureWrapMode::Clamp);
        m_NormalGBuffer->setWrapMode(TextureWrapMode::Clamp);
        m_AlbedoSpecGBuffer->setWrapMode(TextureWrapMode::Clamp);
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

void GeometryBuffers::internalDispose()
{
    glDeleteFramebuffers(1, &m_FboID);
    m_FboID = NULL;

    delete m_PositionGBuffer;
    delete m_NormalGBuffer;
    delete m_DepthTexture;
}
