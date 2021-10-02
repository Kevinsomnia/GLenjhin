#include "Material.h"

Material::Material(Shader* shader) : m_Shader(shader)
{
}

Material::~Material()
{
}

void Material::bind() const
{
	if (m_Shader)
	{
		// TODO: basic shader props here.
		m_Shader->use();
	}
}

void Material::unbind() const
{
	glUseProgram(NULL);
}
