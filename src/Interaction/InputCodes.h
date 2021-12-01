#ifndef INPUTCODES_H
#define INPUTCODES_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum class KeyCode
{
    Unknown,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    Alpha1,
    Alpha2,
    Alpha3,
    Alpha4,
    Alpha5,
    Alpha6,
    Alpha7,
    Alpha8,
    Alpha9,
    Alpha0,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    Space,
    Return,
    Backspace,
    Tab,
    Escape,
    BackQuote,
    LeftCtrl,
    RightCtrl,
    LeftShift,
    RightShift,
    LeftAlt,
    RightAlt,
    CapsLock,
    NumLock,
    ScrollLock,
    Insert,
    Delete,
    Home,
    End,
    PrintScreen,
    Pause,
    PageUp,
    PageDown,
    UpArrow,
    DownArrow,
    LeftArrow,
    RightArrow,
    LeftBracket,
    RightBracket,
    Semicolon,
    Quote,
    Comma,
    Period,
    ForwardSlash,
    BackSlash,
    Minus,
    Equal,
    LeftOSKey,	// Windows key on Windows, etc...
    RightOSKey,
    Menu,
    Keypad1,
    Keypad2,
    Keypad3,
    Keypad4,
    Keypad5,
    Keypad6,
    Keypad7,
    Keypad8,
    Keypad9,
    Keypad0,
    KeypadPlus,
    KeypadMinus,
    KeypadMultiply,
    KeypadDivide,
    KeypadEqual,
    KeypadEnter,
//==========
    LENGTH
};

enum class MouseButton
{
    Unknown,
    Left,
    Right,
    Middle,
//==========
    LENGTH
};

inline int ToGLFWKeyCode(KeyCode keyCode)
{
    switch (keyCode)
    {
        case KeyCode::A:
            return GLFW_KEY_A;
        case KeyCode::B:
            return GLFW_KEY_B;
        case KeyCode::C:
            return GLFW_KEY_C;
        case KeyCode::D:
            return GLFW_KEY_D;
        case KeyCode::E:
            return GLFW_KEY_E;
        case KeyCode::F:
            return GLFW_KEY_F;
        case KeyCode::G:
            return GLFW_KEY_G;
        case KeyCode::H:
            return GLFW_KEY_H;
        case KeyCode::I:
            return GLFW_KEY_I;
        case KeyCode::J:
            return GLFW_KEY_J;
        case KeyCode::K:
            return GLFW_KEY_K;
        case KeyCode::L:
            return GLFW_KEY_L;
        case KeyCode::M:
            return GLFW_KEY_M;
        case KeyCode::N:
            return GLFW_KEY_N;
        case KeyCode::O:
            return GLFW_KEY_O;
        case KeyCode::P:
            return GLFW_KEY_P;
        case KeyCode::Q:
            return GLFW_KEY_Q;
        case KeyCode::R:
            return GLFW_KEY_R;
        case KeyCode::S:
            return GLFW_KEY_S;
        case KeyCode::T:
            return GLFW_KEY_T;
        case KeyCode::U:
            return GLFW_KEY_U;
        case KeyCode::V:
            return GLFW_KEY_V;
        case KeyCode::W:
            return GLFW_KEY_W;
        case KeyCode::X:
            return GLFW_KEY_X;
        case KeyCode::Y:
            return GLFW_KEY_Y;
        case KeyCode::Z:
            return GLFW_KEY_Z;

        case KeyCode::Alpha1:
            return GLFW_KEY_1;
        case KeyCode::Alpha2:
            return GLFW_KEY_2;
        case KeyCode::Alpha3:
            return GLFW_KEY_3;
        case KeyCode::Alpha4:
            return GLFW_KEY_4;
        case KeyCode::Alpha5:
            return GLFW_KEY_5;
        case KeyCode::Alpha6:
            return GLFW_KEY_6;
        case KeyCode::Alpha7:
            return GLFW_KEY_7;
        case KeyCode::Alpha8:
            return GLFW_KEY_8;
        case KeyCode::Alpha9:
            return GLFW_KEY_9;
        case KeyCode::Alpha0:
            return GLFW_KEY_0;

        case KeyCode::F1:
            return GLFW_KEY_F1;
        case KeyCode::F2:
            return GLFW_KEY_F2;
        case KeyCode::F3:
            return GLFW_KEY_F3;
        case KeyCode::F4:
            return GLFW_KEY_F4;
        case KeyCode::F5:
            return GLFW_KEY_F5;
        case KeyCode::F6:
            return GLFW_KEY_F6;
        case KeyCode::F7:
            return GLFW_KEY_F7;
        case KeyCode::F8:
            return GLFW_KEY_F8;
        case KeyCode::F9:
            return GLFW_KEY_F9;
        case KeyCode::F10:
            return GLFW_KEY_F10;
        case KeyCode::F11:
            return GLFW_KEY_F11;
        case KeyCode::F12:
            return GLFW_KEY_F12;

        case KeyCode::Space:
            return GLFW_KEY_SPACE;
        case KeyCode::Return:
            return GLFW_KEY_ENTER;
        case KeyCode::Backspace:
            return GLFW_KEY_BACKSPACE;
        case KeyCode::Tab:
            return GLFW_KEY_TAB;
        case KeyCode::Escape:
            return GLFW_KEY_ESCAPE;
        case KeyCode::BackQuote:
            return GLFW_KEY_GRAVE_ACCENT;

        case KeyCode::LeftCtrl:
            return GLFW_KEY_LEFT_CONTROL;
        case KeyCode::RightCtrl:
            return GLFW_KEY_RIGHT_CONTROL;
        case KeyCode::LeftShift:
            return GLFW_KEY_LEFT_SHIFT;
        case KeyCode::RightShift:
            return GLFW_KEY_RIGHT_SHIFT;
        case KeyCode::LeftAlt:
            return GLFW_KEY_LEFT_ALT;
        case KeyCode::RightAlt:
            return GLFW_KEY_RIGHT_ALT;

        case KeyCode::CapsLock:
            return GLFW_KEY_CAPS_LOCK;
        case KeyCode::NumLock:
            return GLFW_KEY_NUM_LOCK;
        case KeyCode::ScrollLock:
            return GLFW_KEY_SCROLL_LOCK;

        case KeyCode::Insert:
            return GLFW_KEY_INSERT;
        case KeyCode::Delete:
            return GLFW_KEY_DELETE;
        case KeyCode::Home:
            return GLFW_KEY_HOME;
        case KeyCode::End:
            return GLFW_KEY_END;
        case KeyCode::PrintScreen:
            return GLFW_KEY_PRINT_SCREEN;
        case KeyCode::Pause:
            return GLFW_KEY_PAUSE;
        case KeyCode::PageUp:
            return GLFW_KEY_PAGE_UP;
        case KeyCode::PageDown:
            return GLFW_KEY_PAGE_DOWN;

        case KeyCode::UpArrow:
            return GLFW_KEY_UP;
        case KeyCode::DownArrow:
            return GLFW_KEY_DOWN;
        case KeyCode::LeftArrow:
            return GLFW_KEY_LEFT;
        case KeyCode::RightArrow:
            return GLFW_KEY_RIGHT;

        case KeyCode::LeftBracket:
            return GLFW_KEY_LEFT_BRACKET;
        case KeyCode::RightBracket:
            return GLFW_KEY_RIGHT_BRACKET;
        case KeyCode::Semicolon:
            return GLFW_KEY_SEMICOLON;
        case KeyCode::Quote:
            return GLFW_KEY_APOSTROPHE;
        case KeyCode::Comma:
            return GLFW_KEY_COMMA;
        case KeyCode::Period:
            return GLFW_KEY_PERIOD;
        case KeyCode::ForwardSlash:
            return GLFW_KEY_SLASH;
        case KeyCode::BackSlash:
            return GLFW_KEY_BACKSLASH;
        case KeyCode::Minus:
            return GLFW_KEY_MINUS;
        case KeyCode::Equal:
            return GLFW_KEY_EQUAL;
        case KeyCode::LeftOSKey:
            return GLFW_KEY_LEFT_SUPER;
        case KeyCode::RightOSKey:
            return GLFW_KEY_RIGHT_SUPER;
        case KeyCode::Menu:
            return GLFW_KEY_MENU;

        case KeyCode::Keypad1:
            return GLFW_KEY_KP_1;
        case KeyCode::Keypad2:
            return GLFW_KEY_KP_2;
        case KeyCode::Keypad3:
            return GLFW_KEY_KP_3;
        case KeyCode::Keypad4:
            return GLFW_KEY_KP_4;
        case KeyCode::Keypad5:
            return GLFW_KEY_KP_5;
        case KeyCode::Keypad6:
            return GLFW_KEY_KP_6;
        case KeyCode::Keypad7:
            return GLFW_KEY_KP_7;
        case KeyCode::Keypad8:
            return GLFW_KEY_KP_8;
        case KeyCode::Keypad9:
            return GLFW_KEY_KP_9;
        case KeyCode::Keypad0:
            return GLFW_KEY_KP_0;
        case KeyCode::KeypadPlus:
            return GLFW_KEY_KP_ADD;
        case KeyCode::KeypadMinus:
            return GLFW_KEY_KP_SUBTRACT;
        case KeyCode::KeypadMultiply:
            return GLFW_KEY_KP_MULTIPLY;
        case KeyCode::KeypadDivide:
            return GLFW_KEY_KP_DIVIDE;
        case KeyCode::KeypadEqual:
            return GLFW_KEY_KP_EQUAL;
        case KeyCode::KeypadEnter:
            return GLFW_KEY_KP_ENTER;

        default:
            return -1;
    }
}

inline KeyCode FromGLFWKeyCode(int glfwKey)
{
    switch (glfwKey)
    {
        case GLFW_KEY_A:
            return KeyCode::A;
        case GLFW_KEY_B:
            return KeyCode::B;
        case GLFW_KEY_C:
            return KeyCode::C;
        case GLFW_KEY_D:
            return KeyCode::D;
        case GLFW_KEY_E:
            return KeyCode::E;
        case GLFW_KEY_F:
            return KeyCode::F;
        case GLFW_KEY_G:
            return KeyCode::G;
        case GLFW_KEY_H:
            return KeyCode::H;
        case GLFW_KEY_I:
            return KeyCode::I;
        case GLFW_KEY_J:
            return KeyCode::J;
        case GLFW_KEY_K:
            return KeyCode::K;
        case GLFW_KEY_L:
            return KeyCode::L;
        case GLFW_KEY_M:
            return KeyCode::M;
        case GLFW_KEY_N:
            return KeyCode::N;
        case GLFW_KEY_O:
            return KeyCode::O;
        case GLFW_KEY_P:
            return KeyCode::P;
        case GLFW_KEY_Q:
            return KeyCode::Q;
        case GLFW_KEY_R:
            return KeyCode::R;
        case GLFW_KEY_S:
            return KeyCode::S;
        case GLFW_KEY_T:
            return KeyCode::T;
        case GLFW_KEY_U:
            return KeyCode::U;
        case GLFW_KEY_V:
            return KeyCode::V;
        case GLFW_KEY_W:
            return KeyCode::W;
        case GLFW_KEY_X:
            return KeyCode::X;
        case GLFW_KEY_Y:
            return KeyCode::Y;
        case GLFW_KEY_Z:
            return KeyCode::Z;

        case GLFW_KEY_1:
            return KeyCode::Alpha1;
        case GLFW_KEY_2:
            return KeyCode::Alpha2;
        case GLFW_KEY_3:
            return KeyCode::Alpha3;
        case GLFW_KEY_4:
            return KeyCode::Alpha4;
        case GLFW_KEY_5:
            return KeyCode::Alpha5;
        case GLFW_KEY_6:
            return KeyCode::Alpha6;
        case GLFW_KEY_7:
            return KeyCode::Alpha7;
        case GLFW_KEY_8:
            return KeyCode::Alpha8;
        case GLFW_KEY_9:
            return KeyCode::Alpha9;
        case GLFW_KEY_0:
            return KeyCode::Alpha0;

        case GLFW_KEY_F1:
            return KeyCode::F1;
        case GLFW_KEY_F2:
            return KeyCode::F2;
        case GLFW_KEY_F3:
            return KeyCode::F3;
        case GLFW_KEY_F4:
            return KeyCode::F4;
        case GLFW_KEY_F5:
            return KeyCode::F5;
        case GLFW_KEY_F6:
            return KeyCode::F6;
        case GLFW_KEY_F7:
            return KeyCode::F7;
        case GLFW_KEY_F8:
            return KeyCode::F8;
        case GLFW_KEY_F9:
            return KeyCode::F9;
        case GLFW_KEY_F10:
            return KeyCode::F10;
        case GLFW_KEY_F11:
            return KeyCode::F11;
        case GLFW_KEY_F12:
            return KeyCode::F12;

        case GLFW_KEY_SPACE:
            return KeyCode::Space;
        case GLFW_KEY_ENTER:
            return KeyCode::Return;
        case GLFW_KEY_BACKSPACE:
            return KeyCode::Backspace;
        case GLFW_KEY_TAB:
            return KeyCode::Tab;
        case GLFW_KEY_ESCAPE:
            return KeyCode::Escape;
        case GLFW_KEY_GRAVE_ACCENT:
            return KeyCode::BackQuote;

        case GLFW_KEY_LEFT_CONTROL:
            return KeyCode::LeftCtrl;
        case GLFW_KEY_RIGHT_CONTROL:
            return KeyCode::RightCtrl;
        case GLFW_KEY_LEFT_SHIFT:
            return KeyCode::LeftShift;
        case GLFW_KEY_RIGHT_SHIFT:
            return KeyCode::RightShift;
        case GLFW_KEY_LEFT_ALT:
            return KeyCode::LeftAlt;
        case GLFW_KEY_RIGHT_ALT:
            return KeyCode::RightAlt;

        case GLFW_KEY_CAPS_LOCK:
            return KeyCode::CapsLock;
        case GLFW_KEY_NUM_LOCK:
            return KeyCode::NumLock;
        case GLFW_KEY_SCROLL_LOCK:
            return KeyCode::ScrollLock;

        case GLFW_KEY_INSERT:
            return KeyCode::Insert;
        case GLFW_KEY_DELETE:
            return KeyCode::Delete;
        case GLFW_KEY_HOME:
            return KeyCode::Home;
        case GLFW_KEY_END:
            return KeyCode::End;
        case GLFW_KEY_PRINT_SCREEN:
            return KeyCode::PrintScreen;
        case GLFW_KEY_PAUSE:
            return KeyCode::Pause;
        case GLFW_KEY_PAGE_UP:
            return KeyCode::PageUp;
        case GLFW_KEY_PAGE_DOWN:
            return KeyCode::PageDown;

        case GLFW_KEY_UP:
            return KeyCode::UpArrow;
        case GLFW_KEY_DOWN:
            return KeyCode::DownArrow;
        case GLFW_KEY_LEFT:
            return KeyCode::LeftArrow;
        case GLFW_KEY_RIGHT:
            return KeyCode::RightArrow;

        case GLFW_KEY_LEFT_BRACKET:
            return KeyCode::LeftBracket;
        case GLFW_KEY_RIGHT_BRACKET:
            return KeyCode::RightBracket;
        case GLFW_KEY_SEMICOLON:
            return KeyCode::Semicolon;
        case GLFW_KEY_APOSTROPHE:
            return KeyCode::Quote;
        case GLFW_KEY_COMMA:
            return KeyCode::Comma;
        case GLFW_KEY_PERIOD:
            return KeyCode::Period;
        case GLFW_KEY_SLASH:
            return KeyCode::ForwardSlash;
        case GLFW_KEY_BACKSLASH:
            return KeyCode::BackSlash;
        case GLFW_KEY_MINUS:
            return KeyCode::Minus;
        case GLFW_KEY_EQUAL:
            return KeyCode::Equal;
        case GLFW_KEY_LEFT_SUPER:
            return KeyCode::LeftOSKey;
        case GLFW_KEY_RIGHT_SUPER:
            return KeyCode::RightOSKey;
        case GLFW_KEY_MENU:
            return KeyCode::Menu;

        case GLFW_KEY_KP_1:
            return KeyCode::Keypad1;
        case GLFW_KEY_KP_2:
            return KeyCode::Keypad2;
        case GLFW_KEY_KP_3:
            return KeyCode::Keypad3;
        case GLFW_KEY_KP_4:
            return KeyCode::Keypad4;
        case GLFW_KEY_KP_5:
            return KeyCode::Keypad5;
        case GLFW_KEY_KP_6:
            return KeyCode::Keypad6;
        case GLFW_KEY_KP_7:
            return KeyCode::Keypad7;
        case GLFW_KEY_KP_8:
            return KeyCode::Keypad8;
        case GLFW_KEY_KP_9:
            return KeyCode::Keypad9;
        case GLFW_KEY_KP_0:
            return KeyCode::Keypad0;
        case GLFW_KEY_KP_ADD:
            return KeyCode::KeypadPlus;
        case GLFW_KEY_KP_SUBTRACT:
            return KeyCode::KeypadMinus;
        case GLFW_KEY_KP_MULTIPLY:
            return KeyCode::KeypadMultiply;
        case GLFW_KEY_KP_DIVIDE:
            return KeyCode::KeypadDivide;
        case GLFW_KEY_KP_EQUAL:
            return KeyCode::KeypadEqual;
        case GLFW_KEY_KP_ENTER:
            return KeyCode::KeypadEnter;

        default:
            return KeyCode::Unknown;
    }
}

inline int ToGLFWMouseButton(MouseButton button)
{
    switch (button)
    {
        case MouseButton::Left:
            return GLFW_MOUSE_BUTTON_LEFT;
        case MouseButton::Right:
            return GLFW_MOUSE_BUTTON_RIGHT;
        case MouseButton::Middle:
            return GLFW_MOUSE_BUTTON_MIDDLE;
        default:
            return -1;
    }
}

inline MouseButton FromGLFWMouseButton(int glfwButton)
{
    switch (glfwButton)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
            return MouseButton::Left;
        case GLFW_MOUSE_BUTTON_RIGHT:
            return MouseButton::Right;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            return MouseButton::Middle;
        default:
            return MouseButton::Unknown;
    }
}

#endif // INPUTCODES_H
