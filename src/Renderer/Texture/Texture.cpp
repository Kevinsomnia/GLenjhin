#include "Texture.h"

Texture::Texture() : m_Pixels(nullptr), m_TextureID(0), m_Mipmaps(false)
{
}

Texture::~Texture()
{
    if (m_TextureID != 0)
        glDeleteTextures(1, &m_TextureID);

    if (m_Pixels)
        delete[] m_Pixels;
}

void Texture::bind(uint32_t slotIndex) const
{
    glBindTextureUnit(slotIndex, m_TextureID);
}
