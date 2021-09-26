#include "Shader.h"

Shader::Shader(const string& shaderPath) : m_shaderId(0)
{
	ShaderParseResult parsedResults = parseShader(shaderPath);
	m_shaderId = createShader(parsedResults);
}

Shader::~Shader()
{
	glDeleteProgram(m_shaderId);
}

void Shader::use()
{
	glUseProgram(m_shaderId);
}
