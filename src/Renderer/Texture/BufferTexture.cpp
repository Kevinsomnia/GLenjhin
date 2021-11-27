#include "Texture.h"

BufferTexture::BufferTexture(int width, int height, TextureFormat format) : Texture()
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    TextureEnumParams params = TextureEnumParams::FromFormat(format, false);

    glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, params.internalFormat, width, height, 0, params.texFormat, params.valueType, nullptr);
}

BufferTexture::~BufferTexture()
{
}
