#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <GL/glew.h>
#include "../ShaderCompiler.h"

using std::cout;
using std::endl;
using std::string;

class Shader
{
public:
	Shader(const string& shaderPath);
	~Shader();
	void use();
private:
	unsigned int m_shaderId;
};

#endif // SHADER_H
