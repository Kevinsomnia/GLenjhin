#include "Input.h"

Input* Input::m_Inst = nullptr;

void Input::Init(GLFWwindow* window)
{
	if (!m_Inst)
	{
		m_Inst = new Input(window);

		// Immediately zero out the mouse delta.
		Poll();
		m_Inst->m_PrevMousePos = m_Inst->m_CurrMousePos;

		// Register GLFW input callbacks
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scanCode, int action, int mods) {
			if (m_Inst)
			{
				// Note: Not checking GLFW_REPEAT since it doesn't instantly repeat every frame. That behavior will be done in poll().
				if (action == GLFW_PRESS)
				{
					m_Inst->m_RawKeyPressed[(int)FromGLFWKeyCode(key)] = true;
				}
				else if (action == GLFW_RELEASE)
				{
					m_Inst->m_RawKeyPressed[(int)FromGLFWKeyCode(key)] = false;
				}
			}
		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
			if (m_Inst)
			{
				// Note: Not checking GLFW_REPEAT since it doesn't instantly repeat every frame. That behavior will be done in poll().
				if (action == GLFW_PRESS)
				{
					m_Inst->m_RawMouseButtonPressed[(int)FromGLFWMouseButton(button)] = true;
				}
				else if (action == GLFW_RELEASE)
				{
					m_Inst->m_RawMouseButtonPressed[(int)FromGLFWMouseButton(button)] = false;
				}
			}
		});
	}
}

void Input::Poll()
{
	m_Inst->m_PrevMousePos = m_Inst->m_CurrMousePos;

	double x, y;
	glfwGetCursorPos(m_Inst->m_Window, &x, &y);
	m_Inst->m_CurrMousePos = Vector2((float)x, (float)y);

	// Input callbacks are not called every frame, so we need to update them to "held" state accordingly.
	// We should also ensure the press state is set to pressed/released for at least one frame.
	for (size_t i = 0; i < (size_t)KeyCode::LENGTH; i++)
	{
		m_Inst->m_KeyStates[i] = GetNewKeyPressState(m_Inst->m_KeyStates[i], m_Inst->m_RawKeyPressed[i]);
	}

	for (size_t i = 0; i < (size_t)MouseButton::LENGTH; i++)
	{
		m_Inst->m_MouseButtonStates[i] = GetNewKeyPressState(m_Inst->m_MouseButtonStates[i], m_Inst->m_RawMouseButtonPressed[i]);
	}
}

bool Input::GetKey(KeyCode key)
{
	KeyPressState pressState = m_Inst->m_KeyStates[(int)key];
	return pressState != KeyPressState::None && pressState != KeyPressState::Released;
}

bool Input::GetKeyDown(KeyCode key)
{
	KeyPressState pressState = m_Inst->m_KeyStates[(int)key];
	return pressState == KeyPressState::Pressed || pressState == KeyPressState::Tapped;
}

bool Input::GetKeyUp(KeyCode key)
{
	KeyPressState pressState = m_Inst->m_KeyStates[(int)key];
	return pressState == KeyPressState::Released || pressState == KeyPressState::Tapped;
}

Vector2 Input::GetMousePosition()
{
	return m_Inst->m_CurrMousePos;
}

Vector2 Input::GetMouseMoveDelta()
{
	return m_Inst->m_CurrMousePos - m_Inst->m_PrevMousePos;
}

bool Input::GetMouseButton(MouseButton button)
{
	KeyPressState pressState = m_Inst->m_MouseButtonStates[(int)button];
	return pressState != KeyPressState::None && pressState != KeyPressState::Released;
}

bool Input::GetMouseButtonDown(MouseButton button)
{
	KeyPressState pressState = m_Inst->m_MouseButtonStates[(int)button];
	return pressState == KeyPressState::Pressed || pressState == KeyPressState::Tapped;
}

bool Input::GetMouseButtonUp(MouseButton button)
{
	KeyPressState pressState = m_Inst->m_MouseButtonStates[(int)button];
	return pressState == KeyPressState::Released || pressState == KeyPressState::Tapped;
}

KeyPressState Input::GetNewKeyPressState(KeyPressState currentState, bool isKeyDown)
{
	if (isKeyDown)
	{
		switch (currentState)
		{
			case KeyPressState::Pressed:
			case KeyPressState::Held:
				return KeyPressState::Held;
			default:
				return KeyPressState::Pressed;
		}
	}
	else
	{
		switch (currentState)
		{
			case KeyPressState::Pressed:
				return KeyPressState::Tapped;
			case KeyPressState::Held:
				return KeyPressState::Released;
			case KeyPressState::Released:
			case KeyPressState::Tapped:
			case KeyPressState::None:
				return KeyPressState::None;
		}
	}

	return currentState;
}

Input::Input(GLFWwindow* window) : m_Window(window)
{
	memset(m_RawKeyPressed, false, sizeof(m_RawKeyPressed));
	memset(m_KeyStates, (int)KeyPressState::None, sizeof(m_KeyStates));

	memset(m_RawMouseButtonPressed, false, sizeof(m_RawMouseButtonPressed));
	memset(m_MouseButtonStates, (int)KeyPressState::None, sizeof(m_MouseButtonStates));
}

Input::~Input()
{
}
