#include "Material.h"

Material::Material(Shader* shader) : shader(shader)
{
}

Material::~Material()
{
}

void Material::bind() const
{
	if (shader)
	{
		// TODO: basic shader props here.
		shader->use();
	}
}

void Material::unbind() const
{
	glUseProgram(NULL);
}
