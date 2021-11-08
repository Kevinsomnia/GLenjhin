#include "Texture.h"

using ImageLib::PNG;

Texture2D::Texture2D(const std::string& filePath, bool generateMipmaps, bool readable) : Texture()
{
    PNG::Result result = PNG::Load(filePath);

    if (!result.isValid())
        return;

    m_Pixels = result.pixels;
    m_Mipmaps = generateMipmaps;

    bool hasAlpha = result.info.hasAlpha();
    uint32_t texFormatBitDepth = hasAlpha ? GL_RGBA8 : GL_RGB8;
    uint32_t texInternalFormat = hasAlpha ? GL_SRGB_ALPHA : GL_SRGB;
    uint32_t texFormat = hasAlpha ? GL_RGBA : GL_RGB;

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    // TODO: filtering parameter.
    glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, m_Mipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, texInternalFormat, result.info.width, result.info.height, 0, texFormat, GL_UNSIGNED_BYTE, m_Pixels);

    if (m_Mipmaps)
        glGenerateMipmap(GL_TEXTURE_2D);

    // Cleanup after uploading to GPU if we don't need to read it on CPU side.
    if (!readable)
    {
        delete[] m_Pixels;
        m_Pixels = nullptr;
    }
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_TextureID);
}
