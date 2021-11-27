#include "Texture.h"

using ImageLib::PNG;

Texture2D::Texture2D(int width, int height, TextureFormat colorFormat) : Texture()
{
    m_Width = width;
    m_Height = height;
    m_Pixels = nullptr;
    m_Mipmaps = false;

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    setFilterMode(TextureFilterMode::Bilinear);
    setWrapMode(TextureWrapMode::Repeat);

    GLTextureParams params = GLTextureParams::FromFormat(colorFormat, /*sRGB=*/false);
    glTexImage2D(GL_TEXTURE_2D, 0, params.internalFormat, m_Width, m_Height, 0, params.texFormat, params.valueType, m_Pixels);
}

// Import texture from PNG file.
Texture2D::Texture2D(const std::string& filePath, bool generateMipmaps, bool readable) : Texture()
{
    PNG::Result result = PNG::Load(filePath);

    if (result.isValid())
    {
        m_Width = result.info.width;
        m_Height = result.info.height;
        m_Pixels = result.pixels;
        m_Mipmaps = generateMipmaps;
    }
    else
    {
        m_Pixels = nullptr;
        return;
    }

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    setFilterMode(TextureFilterMode::Bilinear);
    setWrapMode(TextureWrapMode::Repeat);

    GLTextureParams params = GLTextureParams::FromFormat(result.info.hasAlpha() ? TextureFormat::RGBA32 : TextureFormat::RGB24, /*sRGB=*/true);
    glTexImage2D(GL_TEXTURE_2D, 0, params.internalFormat, m_Width, m_Height, 0, params.texFormat, params.valueType, m_Pixels);

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
    if (m_Pixels)
        delete[] m_Pixels;
}
