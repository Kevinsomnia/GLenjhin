#include "Texture.h"

using ImageLib::PNG;

Texture2D::Texture2D(const std::string& filePath, bool generateMipmaps, bool readable) : Texture()
{
    PNG::Result result = PNG::Load(filePath);

    if (!result.isValid())
        return;

    m_Pixels = result.pixels;
    m_Mipmaps = generateMipmaps;

    TextureEnumParams params = TextureEnumParams::FromFormat(result.info.hasAlpha() ? TextureFormat::RGBA32 : TextureFormat::RGB24, /*sRGB=*/true);

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    // TODO: filtering parameter.
    glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, m_Mipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, params.internalFormat, result.info.width, result.info.height, 0, params.texFormat, params.valueType, m_Pixels);

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
}
