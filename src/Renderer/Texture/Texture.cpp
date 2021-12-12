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

Vector2 Texture::texelSize() const
{
    return Vector2(1.0f / m_Width, 1.0f / m_Height);
}
