#pragma once

#include <unordered_map>

#include "BufferTexture.h"

#define WRITE_BITS(value, offset, numBits) ((static_cast<uint64_t>(value) & ((1 << numBits) - 1)) << offset)

using std::unordered_multimap;

class BufferTexturePool
{
public:
    static BufferTexture* Get(uint16_t width, uint16_t height, TextureFormat colorFormat, TextureFormat depthFormat, TextureFilterMode filterMode = TextureFilterMode::Bilinear);
    static void Return(BufferTexture* tex);
    static void Clear();
private:
    // TODO: cleanup any textures that weren't used for a while (~10 frames or so)
    static unordered_multimap<uint64_t, BufferTexture*> s_PooledTextures;

    static uint64_t GetMultimapKey(BufferTexture* tex);
    static uint64_t GetMultimapKey(uint16_t width, uint16_t height, TextureFormat colorFormat, TextureFormat depthFormat);
};
