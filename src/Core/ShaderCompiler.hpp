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

namespace ShaderCompiler
{
    enum class ShaderType
    {
        Unknown = -1,
        Vertex = 0,
        Fragment = 1
    };

    struct ParsedPrograms
    {
        const string vertex;
        const string fragment;
    };

    static ParsedPrograms ParseShader(const string& filePath)
    {
        ifstream fs(filePath);
        string line;

        stringstream vert;
        stringstream frag;
        ShaderType shaderType = ShaderType::Unknown;

        while (getline(fs, line))
        {
            if (line.length() == 0)
                continue;

            if (line.find("#pragma vertex") != string::npos)
            {
                shaderType = ShaderType::Vertex;
                continue;
            }
            else if (line.find("#pragma fragment") != string::npos)
            {
                shaderType = ShaderType::Fragment;
                continue;
            }

            switch (shaderType)
            {
                case ShaderType::Vertex:
                    vert << line << '\n';
                    break;
                case ShaderType::Fragment:
                    frag << line << '\n';
                    break;
                default:
                    cerr << "Attempted to parse unknown shader" << endl;
                    break;
            }
        }

        return {
            vert.str(),
            frag.str()
        };
    }

    static uint32_t CompileShader(uint32_t type, const string& source)
    {
        uint32_t id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);

        if (result == GL_FALSE)
        {
            int msgLen;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &msgLen);
            char* msg = (char*)malloc(msgLen * sizeof(char));
            glGetShaderInfoLog(id, msgLen, &msgLen, msg);
            cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << endl;
            cerr << msg << endl;
            free(msg);
            return 0;
        }

        return id;
    }

    static uint32_t CreateShader(const ParsedPrograms& programs)
    {
        uint32_t program = glCreateProgram();
        uint32_t vert = 0;
        uint32_t frag = 0;

        if (programs.vertex.length() > 0)
        {
            vert = CompileShader(GL_VERTEX_SHADER, programs.vertex);
            glAttachShader(program, vert);
        }

        if (programs.fragment.length() > 0)
        {
            frag = CompileShader(GL_FRAGMENT_SHADER, programs.fragment);
            glAttachShader(program, frag);
        }

        glLinkProgram(program);
        glValidateProgram(program);

        // Cleanup after compiling program
        glDeleteShader(vert);
        glDeleteShader(frag);

        return program;
    }
}
