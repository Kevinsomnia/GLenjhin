#include "Texture2D.h"

using ImageLib::PNG;

Texture2D::Texture2D(uint32_t width, uint32_t height, TextureFormat colorFormat, bool readable) : Texture()
{
    m_Width = width;
    m_Height = height;
    m_Mipmaps = false;
    m_Format = colorFormat;

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    setFilterMode(TextureFilterMode::Bilinear);
    setWrapMode(TextureWrapMode::Repeat);

    if (readable)
    {
        m_Pixels = new uint8_t[width * height * 4];
    }
    else
    {
        m_Pixels = nullptr;
    }

    GLTextureParams params = GLTextureParams::FromFormat(m_Format, /*sRGB=*/false);
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

    m_Format = result.info.hasAlpha() ? TextureFormat::RGBA32 : TextureFormat::RGB24;
    GLTextureParams params = GLTextureParams::FromFormat(m_Format, /*sRGB=*/true);
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

ColorByte Texture2D::getPixel(uint32_t x, uint32_t y) const
{
    if (!m_Pixels)
        return nullptr;

    bool hasAlpha = m_Format == TextureFormat::RGBA32;
    uint32_t bpp = hasAlpha ? 4 : 3;
    uint32_t offset = (y * m_Width + x) * bpp;
    return ColorByte(m_Pixels + offset, hasAlpha);
}

ColorByte* Texture2D::getPixels() const
{
    if (!m_Pixels)
        return nullptr;

    ColorByte* result = new ColorByte[m_Width * m_Height];

    // Don't think we can copy the entire uint8_t blob into ColorByte*, but could be worth looking into.
    uint8_t* currPtr = m_Pixels;
    bool hasAlpha = m_Format == TextureFormat::RGBA32;
    uint32_t bpp = hasAlpha ? 4 : 3;

    for (uint32_t y = 0; y < m_Height; y++)
    {
        uint32_t rowStart = y * m_Width;

        for (uint32_t x = 0; x < m_Width; x++)
        {
            uint32_t offset = (rowStart + x) * bpp;
            result[rowStart + x] = ColorByte(m_Pixels + offset, hasAlpha);
        }
    }

    return result;
}

void Texture2D::setPixel(uint32_t x, uint32_t y, const ColorByte& c)
{
    if (!m_Pixels)
        return;

    uint32_t bpp = m_Format == TextureFormat::RGBA32 ? 4 : 3;
    uint32_t offset = (y * m_Width + x) * bpp;
    memcpy(m_Pixels + offset, static_cast<const uint8_t*>(c), bpp);
}

void Texture2D::setPixels(ColorByte* colors)
{
    if (!m_Pixels)
        return;

    // Don't think we can copy the entire uint8_t blob into ColorByte*, but could be worth looking into.
    uint8_t* currPtr = m_Pixels;
    uint32_t bpp = m_Format == TextureFormat::RGBA32 ? 4 : 3;

    for (uint32_t y = 0; y < m_Height; y++)
    {
        uint32_t rowStart = y * m_Width;

        for (uint32_t x = 0; x < m_Width; x++)
        {
            uint32_t offset = (rowStart + x) * bpp;
            memcpy(m_Pixels + offset, reinterpret_cast<uint8_t*>(colors) + offset, bpp);
        }
    }
}

void Texture2D::uploadToGPU(bool keepReadable)
{
    if (!m_Pixels)
        return;

    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    GLTextureParams params = GLTextureParams::FromFormat(m_Format, /*sRGB=*/true);
    glTexImage2D(GL_TEXTURE_2D, 0, params.internalFormat, m_Width, m_Height, 0, params.texFormat, params.valueType, m_Pixels);

    if (m_Mipmaps)
        glGenerateMipmap(GL_TEXTURE_2D);

    if (!keepReadable)
    {
        delete[] m_Pixels;
        m_Pixels = nullptr;
    }
}

Texture2D* Texture2D::whiteTexture = nullptr;

void Texture2D::CreateStaticTextures()
{
    whiteTexture = CreateSolidColorTexture(Color::White());
}

Texture2D* Texture2D::CreateSolidColorTexture(const ColorByte& c)
{
    Texture2D* result = new Texture2D(/*width=*/ 1, /*height=*/ 1, TextureFormat::RGB24, /*readable=*/ true);
    result->setPixel(0, 0, c);
    result->setFilterMode(TextureFilterMode::Point);
    result->setWrapMode(TextureWrapMode::Clamp);
    result->uploadToGPU(/*keepReadable=*/ false);
    return result;
}
