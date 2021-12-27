#include "ShaderCompiler.h"

unordered_map<string, Shader*> ShaderCompiler::s_ShaderCache;


void ShaderCompiler::CompileProgram(const string& filePath, Shader*& output)
{
    auto iter = s_ShaderCache.find(filePath);

    if (iter != s_ShaderCache.end())
    {
        // Use pre-compiled shader.
        output = iter->second;
    }
    else
    {
        InputData inputData = ParseShader(filePath);

        uint32_t program = glCreateProgram();
        uint32_t vert = 0;
        uint32_t frag = 0;

        if (!inputData.vertex.empty())
        {
            vert = CompileShader(GL_VERTEX_SHADER, inputData.vertex);
            glAttachShader(program, vert);
        }

        if (!inputData.fragment.empty())
        {
            frag = CompileShader(GL_FRAGMENT_SHADER, inputData.fragment);
            glAttachShader(program, frag);
        }

        glLinkProgram(program);
        glValidateProgram(program);

        // Cleanup after compiling program
        glDeleteShader(vert);
        glDeleteShader(frag);

        output = new Shader(program);
        output->setName(filePath);
        s_ShaderCache.insert({filePath, output});
    }
}

void ShaderCompiler::CleanCache()
{
    for (auto& pair : s_ShaderCache)
    {
        if (pair.second)
            delete pair.second;
    }

    s_ShaderCache.clear();
}

ShaderCompiler::InputData ShaderCompiler::ParseShader(const string& filePath)
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

    return InputData {
        vert.str(),
        frag.str()
    };
}

uint32_t ShaderCompiler::CompileShader(uint32_t type, const string& source)
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
