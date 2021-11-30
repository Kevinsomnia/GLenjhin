#include "Shader.h"

Shader::Shader(const string& shaderPath)
{
    ShaderParseResult parsedResults = parseShader(shaderPath);
    m_ShaderID = createShader(parsedResults);
}

Shader::~Shader()
{
    glDeleteProgram(m_ShaderID);
}
