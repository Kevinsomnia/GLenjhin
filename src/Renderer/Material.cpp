#include "Material.h"

Material::Material(Shader* shader) : shader(shader)
{
}

Material::~Material()
{
}

void Material::bind()
{
	if (shader)
	{
		// TODO: basic shader props here.
		shader->use();
	}
}

void Material::unbind()
{
	glUseProgram(0);
}
