#include "Texture.h"

Texture::Texture() : m_TextureID(0), m_Width(0), m_Height(0), m_Mipmaps(false)
{
}

Texture::~Texture()
{
    if (m_TextureID != 0)
        glDeleteTextures(1, &m_TextureID);
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
