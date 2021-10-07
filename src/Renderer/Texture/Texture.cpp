#include "Texture.h"

Texture::Texture() : m_TextureID(0), m_BindSlotIndex(0)
{
}

Texture::~Texture()
{
}

void Texture::bind(uint32_t slotIndex)
{
    m_BindSlotIndex = slotIndex;
    glActiveTexture(GL_TEXTURE0 + slotIndex);
    glBindTextureUnit(slotIndex, m_TextureID);
}

int Texture::getBindSlotIndex() const
{
    return m_BindSlotIndex;
}
