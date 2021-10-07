#include "Texture.h"

Texture2D::Texture2D() : Texture()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
    glTextureStorage2D(m_TextureID, 1, GL_RGB8, 4, 2); // no mips for now

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
        0x00, 0x00, 0xFF,
    };

    glTextureSubImage2D(m_TextureID, 0, 0, 0, 4, 2, GL_RGB, GL_UNSIGNED_BYTE, data);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_TextureID);
}
