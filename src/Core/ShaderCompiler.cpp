#include "ShaderCompiler.h"

enum class ShaderType
{
	UNKNOWN = -1, VERTEX = 0, FRAGMENT = 1
};

ShaderParseResult parseShader(const string& filePath)
{
	ifstream fs(filePath);
	string line;

	stringstream vert;
	stringstream frag;
	ShaderType shaderType = ShaderType::UNKNOWN;

	while (getline(fs, line))
	{
		if (line.length() == 0)
			continue;

		if (line.find("#pragma vertex") != string::npos)
		{
			shaderType = ShaderType::VERTEX;
			continue;
		}
		else if (line.find("#pragma fragment") != string::npos)
		{
			shaderType = ShaderType::FRAGMENT;
			continue;
		}

		switch (shaderType)
		{
			case ShaderType::VERTEX:
				vert << line << '\n';
				break;
			case ShaderType::FRAGMENT:
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

unsigned int compileShader(unsigned int type, const string& source)
{
	unsigned int id = glCreateShader(type);
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

unsigned int createShader(const ShaderParseResult& info)
{
	unsigned int program = glCreateProgram();
	unsigned int vert = 0;
	unsigned int frag = 0;

	if (info.vertex.length() > 0)
	{
		vert = compileShader(GL_VERTEX_SHADER, info.vertex);
		glAttachShader(program, vert);
	}

	if (info.fragment.length() > 0)
	{
		frag = compileShader(GL_FRAGMENT_SHADER, info.fragment);
		glAttachShader(program, frag);
	}

	glLinkProgram(program);
	glValidateProgram(program);

	// Cleanup after compiling program
	glDeleteShader(vert);
	glDeleteShader(frag);

	return program;
}
