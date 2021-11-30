#ifndef GEOMETRY_BUFFERS_H
#define GEOMETRY_BUFFERS_H

#include <iostream>
#include "../Texture/Texture.h"
#include "../Material.h"

using std::cout;
using std::cerr;
using std::endl;

class GeometryBuffers
{
public:
    GeometryBuffers(uint32_t width, uint32_t height, uint32_t depth);
    ~GeometryBuffers();
    void setGBufferTextures(Material& mat) const;
    uint32_t id() const { return m_FboID; }
    uint32_t width() const { return m_Width; }
    uint32_t height() const { return m_Height; }
    Texture2D* positionGBuffer() const { return m_PositionGBuffer; }
    Texture2D* normalGBuffer() const { return m_NormalGBuffer; }
    Texture2D* albedoSpecGBuffer() const { return m_AlbedoSpecGBuffer; }
    Texture2D* depthTexture() const { return m_DepthTexture; }
protected:
    uint32_t m_FboID;
    uint32_t m_Width;
    uint32_t m_Height;
    Texture2D* m_PositionGBuffer;
    Texture2D* m_NormalGBuffer;
    Texture2D* m_AlbedoSpecGBuffer;
    Texture2D* m_DepthTexture;

    void internalDispose();
};

#endif  // GEOMETRY_BUFFERS_H
