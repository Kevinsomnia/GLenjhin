#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Macros for error checking
#define ASSERT(x) if(!(x)) __debugbreak();
#define glCheckError(x) glClearError();\
	x;\
	ASSERT(glLogCall(#x, __FILE__, __LINE__));

using std::cout;
using std::endl;

void update();
void cleanup();

// OpenGL error handling
static void glClearError()
{
	while (glGetError());
}

static bool glLogCall(const char* function, const char* file, int lineNr)
{
	while (GLenum error = glGetError())
	{
		cout << "[OpenGL Error] " << "(code " << error << ") at " << file << ':' << lineNr << endl;
		cout << "    function: " << function << endl;
		return false;
	}

	return true;
}

#endif // APPLICATION_H
