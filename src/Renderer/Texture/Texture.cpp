#include "Texture.h"

Texture::Texture()
    : m_TextureID(0), m_Width(0), m_Height(0), m_Mipmaps(false), m_FilterMode(TextureFilterMode::Bilinear), m_WrapMode(TextureWrapMode::Repeat)
{
}

Texture::~Texture()
{
    if (m_TextureID != NULL)
    {
        glDeleteTextures(1, &m_TextureID);
        m_TextureID = NULL;
    }

    GlobalStats::RemoveActiveTexture(this);
}

void Texture::bind(uint32_t slotIndex) const
{
    glBindTextureUnit(slotIndex, m_TextureID);
}

void Texture::setFilterMode(TextureFilterMode filterMode)
{
    if (m_TextureID == 0)
        return;

    m_FilterMode = filterMode;
    glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GetGLTextureFilterMode(filterMode, m_Mipmaps, /*minification=*/ true));
    glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GetGLTextureFilterMode(filterMode, m_Mipmaps, /*minification=*/ false));
}

void Texture::setWrapMode(TextureWrapMode wrapMode)
{
    if (m_TextureID == 0)
        return;

    m_WrapMode = wrapMode;
    GLint mode = GetGLTextureWrapMode(wrapMode);
    glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, mode);
    glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, mode);
}

void Texture::setBorderColor(const Color& c)
{
    if (m_TextureID == 0)
        return;

    // We shouldn't need to store the border color. Don't think there's any reason to get it, but if we do need it later, it's trivial to add.
    glTextureParameterfv(m_TextureID, GL_TEXTURE_BORDER_COLOR, c);
}

void Texture::setAnisotropicFilterLevel(int level)
{
    if (m_TextureID == 0)
        return;

    float filterLevel = Math::Clamp(static_cast<float>(level), 1.0f, 16.0f);
    glTextureParameterf(m_TextureID, GL_TEXTURE_MAX_ANISOTROPY, filterLevel);
}

size_t Texture::memorySizeBytes() const
{
    size_t bpp;

    switch (m_Format)
    {
        case TextureFormat::R8:
            bpp = 1;
            break;
        case TextureFormat::RHalf:
        case TextureFormat::Depth16:
            bpp = 2;
            break;
        case TextureFormat::RGB8:
        case TextureFormat::Depth24:
            bpp = 3;
            break;
        case TextureFormat::RGBA8:
        case TextureFormat::RGB10A2:
        case TextureFormat::RGHalf:
        case TextureFormat::RFloat:
        case TextureFormat::Depth32:
            bpp = 4;
            break;
        case TextureFormat::RGBAHalf:
        case TextureFormat::RGFloat:
            bpp = 8;
            break;
        case TextureFormat::RGBAFloat:
            bpp = 16;
            break;
        default:
            bpp = 0;
            break;
    }

    return m_Width * m_Height * bpp;
}
