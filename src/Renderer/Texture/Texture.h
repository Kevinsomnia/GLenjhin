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

struct TextureEnumParams
{
    GLint internalFormat;
    GLint texFormat;
    GLint valueType;

    static TextureEnumParams FromFormat(TextureFormat format, bool sRGB)
    {
        switch (format)
        {
            case TextureFormat::RGB24:
                return TextureEnumParams { sRGB ? GL_SRGB : GL_RGB, GL_RGB, GL_UNSIGNED_BYTE };
            case TextureFormat::RGBA32:
                return TextureEnumParams { sRGB ? GL_SRGB_ALPHA : GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE };
            case TextureFormat::RGBAHalf:
                return TextureEnumParams { GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT };
            default:
                cerr << "Invalid TextureFormat" << endl;
                return TextureEnumParams();
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
protected:
    uint32_t m_TextureID;
    uint8_t* m_Pixels;
    bool m_Mipmaps;
};

class Texture2D : public Texture
{
public:
    Texture2D(const std::string& filePath, bool generateMipmaps = true, bool readable = false);
    ~Texture2D();
};

class BufferTexture : public Texture
{
public:
    BufferTexture(int width, int height, TextureFormat format);
    ~BufferTexture();
};

#endif  // TEXTURE_H
