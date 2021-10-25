#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Math/Vector.h"
#include "InputCodes.h"

using std::cout;
using std::cerr;
using std::endl;

enum class KeyPressState
{
    None,		// Not pressing
    Pressed,	// Pressed down this frame
    Held,		// Held down LONGER than a frame
    Released,	// Released this frame
    Tapped		// Pressed and released in the same frame
};

enum class MouseCursorState
{
    Default,
    Hidden,
    Locked
};

class Input
{
public:
    static void Init(GLFWwindow* window);
    static void Poll();

    static bool GetKey(KeyCode key);
    static bool GetKeyDown(KeyCode key);
    static bool GetKeyUp(KeyCode key);

    static Vector2 GetMousePosition();
    static Vector2 GetMouseMoveDelta();
    static bool GetMouseButton(MouseButton button);
    static bool GetMouseButtonDown(MouseButton button);
    static bool GetMouseButtonUp(MouseButton button);

    static MouseCursorState GetMouseCursorState();
    static void SetMouseCursorState(MouseCursorState state);
private:
    GLFWwindow* m_Window;
    Vector2 m_CurrMousePos;
    Vector2 m_PrevMousePos;

    bool m_RawKeyPressed[(uint32_t)KeyCode::LENGTH];
    KeyPressState m_KeyStates[(uint32_t)KeyCode::LENGTH];

    bool m_RawMouseButtonPressed[(uint32_t)MouseButton::LENGTH];
    KeyPressState m_MouseButtonStates[(uint32_t)MouseButton::LENGTH];

    Input(GLFWwindow* window);
    ~Input();

    static Input* m_Inst;

    static KeyPressState GetNewKeyPressState(KeyPressState currentState, bool isKeyDown);
};

#endif // INPUT_H
