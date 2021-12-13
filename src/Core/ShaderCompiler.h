#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>

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
uint32_t createShader(const ShaderParseResult& info);
