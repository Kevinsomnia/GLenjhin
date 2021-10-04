#include "Input.h"

Input* Input::m_Inst = nullptr;

void Input::init(GLFWwindow* window)
{
	if (!m_Inst)
	{
		m_Inst = new Input(window);

		// Immediately zero out the mouse delta.
		poll();
		m_Inst->m_PrevMousePos = m_Inst->m_CurrMousePos;
	}
}

void Input::poll()
{
	m_Inst->m_PrevMousePos = m_Inst->m_CurrMousePos;

	double x, y;
	glfwGetCursorPos(m_Inst->m_Window, &x, &y);
	m_Inst->m_CurrMousePos = Vector2((float)x, (float)y);
}

Vector2 Input::getMousePosition()
{
	return m_Inst->m_CurrMousePos;
}

Vector2 Input::getMouseMoveDelta()
{
	return m_Inst->m_CurrMousePos - m_Inst->m_PrevMousePos;
}

Input::Input(GLFWwindow* window) : m_Window(window)
{
}

Input::~Input()
{
}
