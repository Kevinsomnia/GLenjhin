#include "Texture.h"

Texture2D::Texture2D() : Texture()
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char data[8 * 3]
    {
        0x00, 0x00, 0x00,
        0x00, 0x00, 0x00,

        0xFF, 0x00, 0x00,
        0xFF, 0x00, 0x00,
        
        0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00,

        0x00, 0x00, 0xFF,
        0x00, 0x00, 0xFF
    };

    // TODO: mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, 4, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_TextureID);
}
