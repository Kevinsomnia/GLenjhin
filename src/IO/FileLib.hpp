#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>

using std::cout;
using std::cerr;
using std::endl;

namespace FileLib
{
    static bool ReadAllBytes(const std::string& filePath, uint8_t*& data, size_t& dataLen)
    {
        std::ifstream file(filePath, std::ios::binary | std::ios::in);

        if (!file)
        {
            cerr << "Failed to open file: " << filePath << endl;
            return false;
        }

        file.seekg(0, file.end);
        size_t fileSize = static_cast<size_t>(file.tellg());
        file.seekg(0, file.beg);
        std::unique_ptr<char[]> fileData(new char[fileSize]);
        file.read(fileData.get(), fileSize);

        data = reinterpret_cast<uint8_t*>(fileData.release());
        dataLen = fileSize;
        return true;
    }

    inline static bool Exists(const std::string& filePath)
    {
        return std::filesystem::exists(filePath);
    }
}
