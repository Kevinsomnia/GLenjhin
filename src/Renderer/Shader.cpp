#include "Shader.h"

Shader::Shader(const string& shaderPath)
{
    ShaderCompiler::InputData inputData = ShaderCompiler::ParseShader(shaderPath);
    m_ShaderID = ShaderCompiler::CreateShader(inputData);
    setName(inputData.filePath);

    GlobalStats::AddActiveShader(this);
}

Shader::~Shader()
{
    glDeleteProgram(m_ShaderID);
    GlobalStats::RemoveActiveShader(this);
}

void Shader::use() const
{
    glUseProgram(m_ShaderID);
    GlobalStats::AddShaderCall();
}
