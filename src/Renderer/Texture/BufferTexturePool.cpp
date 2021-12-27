#include "BufferTexturePool.h"

unordered_multimap<uint64_t, BufferTexture*> BufferTexturePool::s_PooledTextures;

BufferTexture* BufferTexturePool::Get(uint16_t width, uint16_t height, TextureFormat colorFormat, TextureFormat depthFormat, TextureFilterMode filterMode)
{
    uint64_t key = GetMultimapKey(width, height, colorFormat, depthFormat);
    auto iter = s_PooledTextures.find(key);
    BufferTexture* result;

    if (iter != s_PooledTextures.end())
    {
        result = iter->second;
        s_PooledTextures.erase(iter);
    }
    else
    {
        result = new BufferTexture(width, height, colorFormat, depthFormat);
    }

    result->setFilterMode(filterMode);
    return result;
}

void BufferTexturePool::Return(BufferTexture* tex)
{
    if (!tex)
        return;

    uint64_t key = GetMultimapKey(tex);

    // Check if this texture exists in the map already. You can't return a pooled texture.
    auto valuesRange = s_PooledTextures.equal_range(key);
    for (auto iter = valuesRange.first; iter != valuesRange.second; iter++)
    {
        if (tex == iter->second)
            throw "BufferTexture already exists in the BufferTexturePool!";
    }

    s_PooledTextures.insert({ key, tex });
}

void BufferTexturePool::Clear()
{
    for (auto& pair : s_PooledTextures)
    {
        if (pair.second)
            delete pair.second;
    }

    s_PooledTextures.clear();
}

uint64_t BufferTexturePool::GetMultimapKey(BufferTexture* tex)
{
    Texture2D* depthTex = tex->depthTexture();
    TextureFormat depthFormat = (depthTex) ? depthTex->format() : TextureFormat::None;
    return GetMultimapKey(tex->width(), tex->height(), /*colorFormat=*/ tex->format(), depthFormat);
}

uint64_t BufferTexturePool::GetMultimapKey(uint16_t width, uint16_t height, TextureFormat colorFormat, TextureFormat depthFormat)
{
    uint64_t key = WRITE_BITS(width, 0, 16);
    key |= WRITE_BITS(height, 16, 16);
    key |= WRITE_BITS(colorFormat, 32, 7);
    key |= WRITE_BITS(depthFormat, 39, 7);
    return key;
}
