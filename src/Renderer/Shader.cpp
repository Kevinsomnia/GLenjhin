#include "Shader.h"

uint32_t Shader::s_ActiveShaderID = 0;


Shader::Shader(uint32_t id) : m_ShaderID(id)
{
    GlobalStats::AddActiveShader(this);
}

Shader::~Shader()
{
    glDeleteProgram(m_ShaderID);
    GlobalStats::RemoveActiveShader(this);
}

void Shader::use() const
{
    SetActiveID(m_ShaderID);
}

Shader* Shader::Load(const string& shaderPath)
{
    Shader* result;
    ShaderCompiler::CompileProgram(shaderPath, result);
    return result;
}

uint32_t Shader::GetActiveID()
{
    return s_ActiveShaderID;
}

void Shader::SetActiveID(uint32_t id)
{
    if (s_ActiveShaderID != id)
    {
        s_ActiveShaderID = id;
        glUseProgram(id);
        GlobalStats::AddShaderCall();
    }
}
