#include "Texture.h"

Texture::Texture() : m_TextureID(0), m_Mipmaps(false)
{
}

Texture::~Texture()
{
    if (m_Pixels)
        delete[] m_Pixels;
}

void Texture::bind(uint32_t slotIndex)
{
    glBindTextureUnit(slotIndex, m_TextureID);
}
