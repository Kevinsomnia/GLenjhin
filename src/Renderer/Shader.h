#pragma once

#include <glad/glad.h>
#include "../Core/ShaderCompiler.h"

using std::cout;
using std::endl;
using std::string;

class Shader
{
public:
    Shader(const string& shaderPath);
    ~Shader();
    inline void use() const { glUseProgram(m_ShaderID); }
    inline uint32_t id() const { return m_ShaderID; }
private:
    uint32_t m_ShaderID;
};
