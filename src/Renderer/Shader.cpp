#include "Shader.h"

Shader::Shader(const string& shaderPath)
{
    ShaderCompiler::ParsedPrograms programs = ShaderCompiler::ParseShader(shaderPath);
    m_ShaderID = ShaderCompiler::CreateShader(programs);
}

Shader::~Shader()
{
    glDeleteProgram(m_ShaderID);
}
