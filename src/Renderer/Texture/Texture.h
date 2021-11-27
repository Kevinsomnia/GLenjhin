#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/glew.h>

#include "../../Core/ImageLib.h"

using std::cout;
using std::endl;

class Texture
{
public:
    Texture();
    ~Texture();
    virtual void bind(uint32_t slotIndex);
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

#endif  // TEXTURE_H
