#include "Texture2D.h"

using ImageLib::PNG;

Texture2D::Texture2D(uint32_t width, uint32_t height, TextureFormat colorFormat, bool readable, bool sRGB) : Texture()
{
    m_Width = width;
    m_Height = height;
    m_Mipmaps = false;
    m_Format = colorFormat;
    m_sRGB = sRGB;

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    setFilterMode(TextureFilterMode::Bilinear);
    setWrapMode(TextureWrapMode::Repeat);

    if (readable)
    {
        m_Pixels = new uint8_t[width * height * 4];
    }
    else
    {
        m_Pixels = nullptr;
    }

    GLTextureParams params = GLTextureParams::FromFormat(m_Format, m_sRGB);
    glTexImage2D(GL_TEXTURE_2D, 0, params.internalFormat, m_Width, m_Height, 0, params.texFormat, params.valueType, m_Pixels);
}

// Import texture from PNG file.
Texture2D::Texture2D(const std::string& filePath, bool generateMipmaps, bool readable, bool sRGB) : Texture()
{
    cout << "=== Loading Texture2D from " << filePath << endl;
    m_Pixels = nullptr;
    m_Mipmaps = generateMipmaps;
    m_sRGB = sRGB;

    // Read file data.
    uint8_t* fileData = nullptr;
    size_t fileDataLen = 0;

    if (!FileLib::ReadAllBytes(filePath, fileData, fileDataLen))
    {
        cerr << "Failed to read in Texture2D from file " << filePath << endl;
        return;
    }

    // Compute the hash/key from file data only if the data size is larger than the threshold (tiny textures are likely faster to read directly).
    // If it is found in cache, that means no properties of the texture has changed since last time.
    // If not, then load the file and cache the data for next time.
    bool useCache = fileDataLen >= CACHE_SIZE_THRESHOLD;
    MemoryStream* cacheStream = nullptr;
    // cacheStream will be cleaned up immediately after it's done reading, so this flag may be used afterwards.
    bool cacheHit = false;
    XXH128_hash_t hashKey;

    if (useCache)
    {
        auto props = TextureCache::ItemProperties { fileData, fileDataLen };
        hashKey = TextureCache::GetHash(props);
        cacheStream = TextureCache::ReadFromCache(hashKey);
        cacheHit = cacheStream;
    }

    size_t pixelCount = 0;

    if (useCache && cacheHit)
    {
        m_Width = cacheStream->readUInt32();
        m_Height = cacheStream->readUInt32();
        m_Format = static_cast<TextureFormat>(cacheStream->readUInt16());

        // Remainder of stream is the pixel data.
        assert(cacheStream->byteOffset() < cacheStream->size());
        pixelCount = cacheStream->size() - cacheStream->byteOffset();
        m_Pixels = new uint8_t[pixelCount];
        cacheStream->read(m_Pixels, pixelCount);
        delete cacheStream;

        cout << "Successfully loaded texture data from cache" << endl;
    }
    else
    {
        PNG::Result result = PNG::Load(fileData, fileDataLen);

        if (result.isValid())
        {
            m_Width = result.info.width;
            m_Height = result.info.height;
            m_Pixels = result.pixels;
            m_Format = result.info.hasAlpha() ? TextureFormat::RGBA8 : TextureFormat::RGB8;

            pixelCount = result.pixelCount;
        }
    }

    // Done reading files at this point.
    if (fileData)
        delete[] fileData;

    if (pixelCount != 0)
    {
        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);

        // Set default parameters.
        setFilterMode(TextureFilterMode::Bilinear);
        setWrapMode(TextureWrapMode::Repeat);

        GLTextureParams params = GLTextureParams::FromFormat(m_Format, m_sRGB);
        glTexImage2D(GL_TEXTURE_2D, 0, params.internalFormat, m_Width, m_Height, 0, params.texFormat, params.valueType, m_Pixels);

        if (m_Mipmaps)
            glGenerateMipmap(GL_TEXTURE_2D);

        if (useCache && !cacheHit)
        {
            // Write properties and pixel data to cache.
            MemoryStream stream;
            stream.writeUInt32(m_Width);
            stream.writeUInt32(m_Height);
            stream.writeUInt16(static_cast<uint16_t>(m_Format));
            stream.write(m_Pixels, pixelCount);
            TextureCache::WriteToCache(hashKey, stream);
        }
    }

    // Cleanup after uploading to GPU if we don't need to read it on CPU side. Or if this catastrophically fails.
    if (m_Pixels && (!readable || pixelCount == 0))
    {
        delete[] m_Pixels;
        m_Pixels = nullptr;
    }
}

Texture2D::~Texture2D()
{
    if (m_Pixels)
        delete[] m_Pixels;
}

ColorByte Texture2D::getPixel(uint32_t x, uint32_t y) const
{
    if (!m_Pixels)
        return nullptr;

    bool hasAlpha = m_Format == TextureFormat::RGBA8;
    uint32_t bpp = hasAlpha ? 4 : 3;
    uint32_t offset = (y * m_Width + x) * bpp;
    return ColorByte(m_Pixels + offset, hasAlpha);
}

ColorByte* Texture2D::getPixels() const
{
    if (!m_Pixels)
        return nullptr;

    ColorByte* result = new ColorByte[m_Width * m_Height];

    // Don't think we can copy the entire uint8_t blob into ColorByte*, but could be worth looking into.
    uint8_t* currPtr = m_Pixels;
    bool hasAlpha = m_Format == TextureFormat::RGBA8;
    uint32_t bpp = hasAlpha ? 4 : 3;

    for (uint32_t y = 0; y < m_Height; y++)
    {
        uint32_t rowStart = y * m_Width;

        for (uint32_t x = 0; x < m_Width; x++)
        {
            uint32_t offset = (rowStart + x) * bpp;
            result[rowStart + x] = ColorByte(m_Pixels + offset, hasAlpha);
        }
    }

    return result;
}

void Texture2D::setPixel(uint32_t x, uint32_t y, const ColorByte& c)
{
    if (!m_Pixels)
        return;

    uint32_t bpp = m_Format == TextureFormat::RGBA8 ? 4 : 3;
    uint32_t offset = (y * m_Width + x) * bpp;
    memcpy(m_Pixels + offset, static_cast<const uint8_t*>(c), bpp);
}

void Texture2D::setPixels(ColorByte* colors)
{
    if (!m_Pixels)
        return;

    // Don't think we can copy the entire uint8_t blob into ColorByte*, but could be worth looking into.
    uint8_t* currPtr = m_Pixels;
    uint32_t bpp = m_Format == TextureFormat::RGBA8 ? 4 : 3;

    for (uint32_t y = 0; y < m_Height; y++)
    {
        uint32_t rowStart = y * m_Width;

        for (uint32_t x = 0; x < m_Width; x++)
        {
            uint32_t offset = (rowStart + x) * bpp;
            memcpy(m_Pixels + offset, reinterpret_cast<uint8_t*>(colors) + offset, bpp);
        }
    }
}

void Texture2D::uploadToGPU(bool keepReadable)
{
    if (!m_Pixels)
        return;

    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    GLTextureParams params = GLTextureParams::FromFormat(m_Format, m_sRGB);
    glTexImage2D(GL_TEXTURE_2D, 0, params.internalFormat, m_Width, m_Height, 0, params.texFormat, params.valueType, m_Pixels);

    if (m_Mipmaps)
        glGenerateMipmap(GL_TEXTURE_2D);

    if (!keepReadable)
    {
        delete[] m_Pixels;
        m_Pixels = nullptr;
    }
}

Texture2D* Texture2D::blackTexture = nullptr;
Texture2D* Texture2D::whiteTexture = nullptr;
Texture2D* Texture2D::normalMapDefaultTexture = nullptr;
Texture2D* Texture2D::msaDefaultTexture = nullptr;

void Texture2D::CreateStaticTextures()
{
    blackTexture = CreateSolidColorTexture(Color::Black(), /*sRGB=*/ true);
    whiteTexture = CreateSolidColorTexture(Color::White(), /*sRGB=*/ true);
    normalMapDefaultTexture = CreateSolidColorTexture(ColorByte(128, 128, 255), /*sRGB=*/ false);
    msaDefaultTexture = CreateSolidColorTexture(Color(0.0f, 0.8f, 1.0f), /*sRGB=*/ true);
}

Texture2D* Texture2D::CreateSolidColorTexture(const ColorByte& c, bool sRGB)
{
    Texture2D* result = new Texture2D(/*width=*/ 1, /*height=*/ 1, TextureFormat::RGB8, /*readable=*/ true, sRGB);
    result->setPixel(0, 0, c);
    result->setFilterMode(TextureFilterMode::Point);
    result->setWrapMode(TextureWrapMode::Clamp);
    result->uploadToGPU(/*keepReadable=*/ false);
    return result;
}
