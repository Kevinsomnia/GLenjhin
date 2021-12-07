#ifndef TEXTURE_H
#define TEXTURE_H

#include <assert.h>
#include <iostream>
#include <glad/glad.h>

#include "../../IO/ImageLib.h"
#include "../../Math/Vector.h"
#include "../Color.h"

using std::cout;
using std::cerr;
using std::endl;

enum class TextureFormat
{
    None,       // Can be used to signify disabled/unused texture
    RGB24,      // 8 bits per channel, RGB
    RGBA32,     // 8 bits per channel, RGBA
    RGBAHalf,   // 16 bits per channel (floating point), RGBA
    RGBAFloat,  // 32 bits per channel (floating point), RGBA
    Depth16,    // 16-bit floating point depth texture
    Depth24,    // 24-bit floating point depth texture
    Depth32     // 32-bit floating point depth texture
};

static TextureFormat GetDepthTextureFormat(uint8_t depth)
{
    switch (depth)
    {
        case 32:
            return TextureFormat::Depth32;
        case 24:
            return TextureFormat::Depth24;
        case 0:
            return TextureFormat::None;
        default:
            return TextureFormat::Depth16;
    }
}

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
    Bilinear,   // GL_LINEAR / GL_LINEAR_MIPMAP_NEAREST
    Trilinear   // GL_LINEAR / GL_LINEAR_MIPMAP_LINEAR
};

static GLint GetGLTextureFilterMode(TextureFilterMode mode, bool mipmap = false, bool minification = true)
{
    switch (mode)
    {
        case TextureFilterMode::Point:
            return mipmap && minification ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
        case TextureFilterMode::Bilinear:
            return mipmap && minification ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;
        case TextureFilterMode::Trilinear:
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
            case TextureFormat::RGBAFloat:
                return GLTextureParams { GL_RGBA32F, GL_RGBA, GL_FLOAT };
            case TextureFormat::Depth16:
                return GLTextureParams { GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_HALF_FLOAT };
            case TextureFormat::Depth24:
                return GLTextureParams { GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT };
            case TextureFormat::Depth32:
                return GLTextureParams { GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT };
            case TextureFormat::None:
                cerr << "You must provide a TextureFormat for a valid conversion" << endl;
                return GLTextureParams { GL_NONE, GL_NONE, GL_NONE };
            default:
                cerr << "Unimplemented TextureFormat to TextureGLParams conversion " << static_cast<uint32_t>(format) << endl;
                return GLTextureParams { GL_NONE, GL_NONE, GL_NONE };
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
    Vector2 texelSize() const;
protected:
    uint32_t m_TextureID;
    uint8_t* m_Pixels;
    uint32_t m_Width;
    uint32_t m_Height;
    bool m_Mipmaps;
    TextureFormat m_TextureFormat;
    TextureFilterMode m_FilterMode;
    TextureWrapMode m_WrapMode;
};

class Texture2D : public Texture
{
public:
    static Texture2D* whiteTexture;

    Texture2D(uint32_t width, uint32_t height, TextureFormat colorFormat, bool readable);
    Texture2D(const std::string& filePath, bool generateMipmaps, bool readable);
    ~Texture2D();

    ColorByte getPixel(uint32_t x, uint32_t y) const;
    ColorByte* getPixels() const;
    void setPixel(uint32_t x, uint32_t y, const ColorByte& c);
    void setPixels(ColorByte* colors);
    void uploadToGPU(bool keepReadable);

    static void CreateStaticTextures();
private:
    static Texture2D* CreateSolidColorTexture(const ColorByte& c);
};

class BufferTexture : public Texture
{
public:
    BufferTexture(uint32_t width, uint32_t height, uint8_t depth, TextureFormat colorFormat);
    ~BufferTexture();
    Texture2D* colorTexture() const;
    Texture2D* depthTexture() const;
    void bind(uint32_t slotIndex) const override;
    void setFilterMode(TextureFilterMode filterMode) override;
    void setWrapMode(TextureWrapMode wrapMode) override;
protected:
    Texture2D* m_ColorTexture;
    Texture2D* m_DepthTexture;

    void internalDispose();
};

#endif  // TEXTURE_H
