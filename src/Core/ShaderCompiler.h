#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <glad/glad.h>

#include "../Renderer/Shader.h"

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::string;
using std::unordered_map;


// Forward declarations
class Shader;


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
        const string vertex;
        const string fragment;
    };

    static void CompileProgram(const string& filePath, Shader*& output);
    static void CleanCache();
private:
    static unordered_map<string, Shader*> s_ShaderCache;

    static InputData ParseShader(const string& filePath);
    static uint32_t CompileShader(uint32_t type, const string& source);
};
