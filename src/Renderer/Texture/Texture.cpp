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
