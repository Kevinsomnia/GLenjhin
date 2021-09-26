#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <GL/glew.h>
#include "Shader.h"

using std::cout;
using std::endl;

class Material
{
public:
	Material(Shader* shader);
	~Material();
	void bind();
	void unbind();
private:
	Shader* shader;
};

#endif // MATERIAL_H
