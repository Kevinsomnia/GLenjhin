#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <GL/glew.h>
#include "../Core/ShaderCompiler.h"

using std::cout;
using std::endl;
using std::string;

class Shader
{
public:
    Shader(const string& shaderPath);
    ~Shader();
    void use() const;
    const uint32_t getShaderId() const;
private:
    uint32_t m_ShaderId;
};

#endif // SHADER_H
