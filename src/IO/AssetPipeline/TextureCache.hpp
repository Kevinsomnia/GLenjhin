#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "../../Renderer/Texture/Texture.h"
#include "../FileLib.hpp"
#include "xxhash.h"

class TextureCache
{
public:
    static const uint64_t CACHE_VERSION = 0;   // increment to invalidate all cache (used as seed).
    inline static const std::string CACHE_PATH = "app_cache\\textures\\";

    struct ItemProperties
    {
        // TODO: texture format stuff (compression, BPP, etc)
        uint8_t* sourceData;
        size_t sourceDataLen;
    };

    static XXH128_hash_t GetHash(ItemProperties& props)
    {
        return XXH3_128bits_withSeed(props.sourceData, props.sourceDataLen, /*seed=*/ CACHE_VERSION);
    }

    static MemoryStream* ReadFromCache(XXH128_hash_t& hash)
    {
        EnsureCacheDirectoryExists();
        std::string filePath = CACHE_PATH + GetHashFilename(hash);

        if (FileLib::Exists(filePath))
        {
            uint8_t* data; size_t dataLen;
            FileLib::ReadAllBytes(filePath, data, dataLen);
            return new MemoryStream(data, dataLen, /*deleteBufferOnDestroy=*/ true);
        }
        else
        {
            return nullptr;
        }
    }

    static void WriteToCache(XXH128_hash_t& hash, MemoryStream& stream)
    {
        EnsureCacheDirectoryExists();
        std::string filePath = CACHE_PATH + GetHashFilename(hash);
        std::ofstream outFile(filePath, std::ios::binary | std::ios::out);
        stream.dumpToOstream(outFile);
        cout << "Cached texture data to " << filePath << endl;
    }

private:
    static void EnsureCacheDirectoryExists()
    {
        std::filesystem::create_directories(std::filesystem::path(CACHE_PATH));
    }

    static std::string GetHashFilename(XXH128_hash_t hash)
    {
        std::stringstream hexString;
        size_t hexLen = sizeof(uint64_t) * 2;
        hexString << std::setfill('0') << std::setw(hexLen) << std::hex << hash.high64;
        hexString << std::setfill('0') << std::setw(hexLen) << std::hex << hash.low64;
        return hexString.str();
    }
};
