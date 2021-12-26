#include "Shader.h"

Shader::Shader(const string& shaderPath)
{
    ShaderCompiler::InputData inputData = ShaderCompiler::ParseShader(shaderPath);
    m_ShaderID = ShaderCompiler::CreateShader(inputData);
    setName(inputData.filePath);
}

Shader::~Shader()
{
    glDeleteProgram(m_ShaderID);
}
