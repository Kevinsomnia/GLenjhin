#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Math/Vector.h"

using std::cout;
using std::endl;

class Input
{
public:
	static void init(GLFWwindow* window);
	static void poll();
	static Vector2 getMousePosition();
	static Vector2 getMouseMoveDelta();
private:
	static Input* m_Inst;

	GLFWwindow* m_Window;
	Vector2 m_CurrMousePos;
	Vector2 m_PrevMousePos;

	Input(GLFWwindow* window);
	~Input();
};

#endif // INPUT_H
