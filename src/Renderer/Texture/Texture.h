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

enum class TextureWrapMode
{
    Repeat, // GL_REPEAT
    Clamp,  // GL_CLAMP_TO_EDGE
    Mirror  // GL_MIRRORED_REPEAT
};

static GLint GetGLTextureWrapMode(TextureWrapMode mode)
{
    switch (mode)
    {
        case TextureWrapMode::Repeat:
            return GL_REPEAT;
        case TextureWrapMode::Clamp:
            return GL_CLAMP_TO_EDGE;
        case TextureWrapMode::Mirror:
            return GL_MIRRORED_REPEAT;
        default:
            cerr << "Unimplemented TextureWrapMode conversion " << static_cast<uint32_t>(mode) << endl;
            return GL_NONE;
    }
}

enum class TextureFilterMode
{
    Point,      // GL_NEAREST
    Bilinear,   // GL_LINEAR
    // Trilinear TODO: make this functional
};

static GLint GetGLTextureFilterMode(TextureFilterMode mode, bool mipmap = false, bool minification = true)
{
    switch (mode)
    {
        case TextureFilterMode::Point:
            return mipmap && minification ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
        case TextureFilterMode::Bilinear:
            return mipmap && minification ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
        default:
            cerr << "Unimplemented TextureFilterMode conversion " << static_cast<uint32_t>(mode) << endl;
            return GL_NONE;
    }
}

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
                cerr << "Unimplemented TextureFormat to TextureGLParams conversion " << static_cast<uint32_t>(format) << endl;
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
    virtual void setFilterMode(TextureFilterMode filterMode);
    virtual void setWrapMode(TextureWrapMode wrapMode);
    uint32_t id() const { return m_TextureID; }
    uint32_t width() const { return m_Width; }
    uint32_t height() const { return m_Height; }
protected:
    uint32_t m_TextureID;
    uint8_t* m_Pixels;
    uint32_t m_Width;
    uint32_t m_Height;
    bool m_Mipmaps;
    TextureFilterMode m_FilterMode;
    TextureWrapMode m_WrapMode;
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
    BufferTexture(int width, int height, int depth, TextureFormat colorFormat);
    ~BufferTexture();
    Texture2D* colorTexture() const;
    void bind(uint32_t slotIndex) const override;
    void setFilterMode(TextureFilterMode filterMode) override;
    void setWrapMode(TextureWrapMode wrapMode) override;
protected:
    Texture2D* m_ColorTexture;
    uint32_t m_DepthTextureID;  // prob will be an readable texture later

    void internalDispose();
};

#endif  // TEXTURE_H
