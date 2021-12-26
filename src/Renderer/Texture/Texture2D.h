#pragma once

#include <filesystem>

#include "Texture.h"
#include "../../IO/AssetPipeline/TextureCache.hpp"

class Texture2D : public Texture
{
public:
    static Texture2D* blackTexture;
    static Texture2D* whiteTexture;
    static Texture2D* normalMapDefaultTexture;
    static Texture2D* msaDefaultTexture;

    Texture2D(uint32_t width, uint32_t height, TextureFormat colorFormat, bool readable, bool sRGB);
    Texture2D(const std::string& filePath, bool generateMipmaps, bool readable, bool sRGB);
    ~Texture2D();

    ColorByte getPixel(uint32_t x, uint32_t y) const;
    ColorByte* getPixels() const;
    void setPixel(uint32_t x, uint32_t y, const ColorByte& c);
    void setPixels(ColorByte* colors);
    void uploadToGPU(bool keepReadable);

    static void CreateStaticTextures();
private:
    static Texture2D* CreateSolidColorTexture(const ColorByte& c, bool sRGB);
};
