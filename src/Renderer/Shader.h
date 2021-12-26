#pragma once

#include <glad/glad.h>
#include "../Core/ShaderCompiler.h"
#include "GlobalStats.h"

using std::cout;
using std::endl;
using std::string;


// Forward declarations
class GlobalStats;


class Shader
{
public:
    Shader(const string& shaderPath);
    ~Shader();
    void use() const;
    inline string name() const { return m_Name; }
    inline uint32_t id() const { return m_ShaderID; }
    inline void setName(const string& name) { m_Name = name; }

    static uint32_t GetActiveID();
    static void SetActiveID(uint32_t id);
private:
    static uint32_t s_ActiveShaderID;

    string m_Name;
    uint32_t m_ShaderID;
};
