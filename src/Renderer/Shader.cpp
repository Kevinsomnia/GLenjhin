#include "Shader.h"

Shader::Shader(const string& shaderPath) : m_ShaderId(0)
{
	ShaderParseResult parsedResults = parseShader(shaderPath);
	m_ShaderId = createShader(parsedResults);
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderId);
}

void Shader::use() const
{
	glUseProgram(m_ShaderId);
}
