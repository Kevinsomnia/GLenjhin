#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::string;

class ShaderCompiler
{
public:
    enum class ShaderType
    {
        Unknown = -1,
        Vertex = 0,
        Fragment = 1
    };

    struct InputData
    {
        const string filePath;
        const string vertex;
        const string fragment;
    };

    static InputData ParseShader(const string& filePath);
    static uint32_t CompileShader(uint32_t type, const string& source);
    static uint32_t CreateShader(const InputData& input);
};
