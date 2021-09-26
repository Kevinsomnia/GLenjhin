#ifndef SHADER_COMPILER_H
#define SHADER_COMPILER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::string;

struct ShaderParseResult
{
	const string vertex;
	const string fragment;
};

ShaderParseResult parseShader(const string& filePath);
unsigned int createShader(const ShaderParseResult& info);

#endif // SHADER_COMPILER_H