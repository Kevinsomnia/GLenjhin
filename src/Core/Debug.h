#pragma once

#include <fstream>
#include <string>

namespace Debug
{
    static void DumpBufferToFile(const std::string& filePath, uint8_t* buf, size_t bufSize)
    {
        std::ofstream file(filePath, std::ios::binary | std::ios::out);
        file.write(reinterpret_cast<char*>(buf), bufSize);
    }
}
