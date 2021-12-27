#include "GeometryBuffers.h"

GeometryBuffers::GeometryBuffers(uint32_t width, uint32_t height, TextureFormat depthFormat, bool motionVectors) : m_Width(width), m_Height(height)
{
    glGenFramebuffers(1, &m_FboID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);

    std::vector<GLenum> colorAttachments = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };

    m_PositionGBuffer = new Texture2D(width, height, TextureFormat::RGBAFloat, /*readable=*/ false, /*sRGB=*/ false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PositionGBuffer->id(), 0);
    m_NormalSmoothGBuffer = new Texture2D(width, height, TextureFormat::RGBAHalf, /*readable=*/ false, /*sRGB=*/ false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_NormalSmoothGBuffer->id(), 0);
    m_AlbedoMetalGBuffer = new Texture2D(width, height, TextureFormat::RGBA8, /*readable=*/ false, /*sRGB=*/ false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_AlbedoMetalGBuffer->id(), 0);
    m_EmissionOcclGBuffer = new Texture2D(width, height, TextureFormat::RGBAHalf, /*readable=*/ false, /*sRGB=*/ false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_EmissionOcclGBuffer->id(), 0);

    if (motionVectors)
    {
        m_MotionVectorsTexture = new Texture2D(width, height, TextureFormat::RGHalf, /*readable=*/ false, /*sRGB=*/ false);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_MotionVectorsTexture->id(), 0);
        colorAttachments.push_back(GL_COLOR_ATTACHMENT4);
    }

    glDrawBuffers(static_cast<uint32_t>(colorAttachments.size()), colorAttachments.data());

    if (depthFormat != TextureFormat::None)
    {
        assert(depthFormat == TextureFormat::Depth16 || depthFormat == TextureFormat::Depth24 || depthFormat == TextureFormat::Depth32);

        // Depth texture should always have point filtering.
        m_DepthTexture = new Texture2D(width, height, depthFormat, /*readable=*/ false, /*sRGB=*/ false);
        m_DepthTexture->setFilterMode(TextureFilterMode::Point);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture->id(), 0);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        // Finish setup
        m_PositionGBuffer->setFilterMode(TextureFilterMode::Point);
        m_NormalSmoothGBuffer->setFilterMode(TextureFilterMode::Point);
        m_AlbedoMetalGBuffer->setFilterMode(TextureFilterMode::Point);
        m_EmissionOcclGBuffer->setFilterMode(TextureFilterMode::Point);

        m_PositionGBuffer->setWrapMode(TextureWrapMode::Clamp);
        m_NormalSmoothGBuffer->setWrapMode(TextureWrapMode::Clamp);
        m_AlbedoMetalGBuffer->setWrapMode(TextureWrapMode::Clamp);
        m_EmissionOcclGBuffer->setWrapMode(TextureWrapMode::Clamp);

        if (m_MotionVectorsTexture)
        {
            m_MotionVectorsTexture->setFilterMode(TextureFilterMode::Point);
            m_MotionVectorsTexture->setWrapMode(TextureWrapMode::Clamp);
        }
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
    GlobalStats::AddFramebufferBindCall();
}

void GeometryBuffers::setGBufferTextures(Material& mat) const
{
    mat.setTexture("u_Position", m_PositionGBuffer);
    mat.setTexture("u_NormalSmooth", m_NormalSmoothGBuffer);
    mat.setTexture("u_AlbedoMetal", m_AlbedoMetalGBuffer);
    mat.setTexture("u_EmissionOccl", m_EmissionOcclGBuffer);
}

void GeometryBuffers::internalDispose()
{
    glDeleteFramebuffers(1, &m_FboID);
    m_FboID = NULL;

    delete m_PositionGBuffer;
    delete m_NormalSmoothGBuffer;
    delete m_AlbedoMetalGBuffer;
    delete m_EmissionOcclGBuffer;

    if (m_DepthTexture)
        delete m_DepthTexture;
    if (m_MotionVectorsTexture)
        delete m_MotionVectorsTexture;
}
