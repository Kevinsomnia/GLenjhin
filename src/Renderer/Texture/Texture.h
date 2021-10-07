#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/glew.h>

using std::cout;
using std::endl;

class Texture
{
public:
    Texture();
    ~Texture();
    virtual void bind(uint32_t slotIndex);
    int getBindSlotIndex() const;
protected:
    uint32_t m_TextureID;
    uint32_t m_BindSlotIndex;
};

class Texture2D : public Texture
{
public:
    Texture2D();
    ~Texture2D();
};

#endif  // TEXTURE_H
