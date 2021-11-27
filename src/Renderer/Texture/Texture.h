#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/glew.h>

#include "../../Core/ImageLib.h"

using std::cout;
using std::cerr;
using std::endl;

enum class TextureFormat
{
    RGB24,      // 8 bits per channel, RGB
    RGBA32,     // 8 bits per channel, RGBA
    RGBAHalf    // 16 bits per channel (floating point), RGBA
};

struct GLTextureParams
{
    GLint internalFormat;
    GLint texFormat;
    GLint valueType;

    static GLTextureParams FromFormat(TextureFormat format, bool sRGB)
    {
        switch (format)
        {
            case TextureFormat::RGB24:
                return GLTextureParams { sRGB ? GL_SRGB : GL_RGB, GL_RGB, GL_UNSIGNED_BYTE };
            case TextureFormat::RGBA32:
                return GLTextureParams { sRGB ? GL_SRGB_ALPHA : GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE };
            case TextureFormat::RGBAHalf:
                return GLTextureParams { GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT };
            default:
                cerr << "Invalid TextureFormat: cannot convert from TextureFormat to TextureGLParams" << endl;
                return GLTextureParams();
        }
    }
};

class Texture
{
public:
    Texture();
    ~Texture();
    virtual void bind(uint32_t slotIndex) const;
    uint32_t id() const { return m_TextureID; }
    uint32_t width() const { return m_Width; }
    uint32_t height() const { return m_Height; }
protected:
    uint32_t m_TextureID;
    uint8_t* m_Pixels;
    uint32_t m_Width;
    uint32_t m_Height;
    bool m_Mipmaps;
};

class Texture2D : public Texture
{
public:
    Texture2D(int width, int height, TextureFormat colorFormat);
    Texture2D(const std::string& filePath, bool generateMipmaps = true, bool readable = false);
    ~Texture2D();
};

class BufferTexture : public Texture
{
public:
    BufferTexture(int width, int height, TextureFormat colorFormat);
    ~BufferTexture();
};

#endif  // TEXTURE_H
