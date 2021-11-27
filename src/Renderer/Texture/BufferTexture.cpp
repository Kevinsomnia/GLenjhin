#include "Texture.h"

BufferTexture::BufferTexture(int width, int height, TextureFormat colorFormat) : Texture()
{
    m_Width = width;
    m_Height = height;

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    // DEFAULT: linear, clamp
    glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLTextureParams params = GLTextureParams::FromFormat(colorFormat, false);
    glTexImage2D(GL_TEXTURE_2D, 0, params.internalFormat, m_Width, m_Height, 0, params.texFormat, params.valueType, nullptr);
}

BufferTexture::~BufferTexture()
{
}
