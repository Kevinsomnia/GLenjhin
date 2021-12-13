#pragma once

#include "Texture.h"

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
