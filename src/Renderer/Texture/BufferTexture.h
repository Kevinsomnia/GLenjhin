#pragma once

#include "Texture2D.h"

class BufferTexture : public Texture
{
public:
    BufferTexture(uint32_t width, uint32_t height, uint8_t depth, TextureFormat colorFormat);
    ~BufferTexture();
    Texture2D* colorTexture() const;
    Texture2D* depthTexture() const;
    void bind() const;
    void bind(uint32_t slotIndex) const override;
    void setFilterMode(TextureFilterMode filterMode) override;
    void setWrapMode(TextureWrapMode wrapMode) override;
protected:
    Texture2D* m_ColorTexture;
    Texture2D* m_DepthTexture;

    void internalDispose();
};
