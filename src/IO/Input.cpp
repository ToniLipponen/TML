#include <TML/IO/Input.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <map>
#include <iostream>
// Keycode and action

static tml::ui32 s_keyboardLastKey = -1;
static tml::ui32 s_keyboardChar = 0; // Last codepoint.
static std::map<tml::i32, tml::i32> s_keyMap;

static std::map<tml::i32, tml::i32> s_mouseButtonMap;
static std::wstring s_string;
static double s_mouseScrollValue = 0;
static tml::Vector2 s_mousePos;

extern "C" void MouseMoveCallback(GLFWwindow* window, double x, double y)
{
    s_mousePos.x = x;
    s_mousePos.y = y;
}

extern "C" void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    s_mouseScrollValue = yoffset;
}

extern "C" void CharCallback(GLFWwindow* window, unsigned int code)
{
    s_string.push_back(static_cast<wchar_t>(code));
    s_keyboardChar = code;
}

extern "C" void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_BACKSPACE && !s_string.empty())
        s_string.pop_back();
    s_keyboardLastKey = key;
    s_keyMap[key] = action;
}

extern "C" void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    s_mouseButtonMap[button] = action;
}

namespace tml
{
    void Keyboard::BeginString()
    {
        s_string.clear();
    }
    std::wstring Keyboard::EndString()
    {
        std::wstring s = s_string;
        s_string.clear();
        return s;
    }
    const std::wstring& Keyboard::GetString()
    {
        return s_string;
    }

    ui32 Keyboard::GetChar()
    {
        return s_keyboardChar;
    }

    ui32 Keyboard::GetKey()
    {
        auto key = s_keyboardLastKey;
        s_keyboardLastKey = -1;
        return key;
    }
    bool Keyboard::IsKeyPressed(Key key)
    {
        bool state = s_keyMap[key] == GLFW_PRESS;
        if(state)
            s_keyMap[key] = GLFW_RELEASE;
        return state;
    }

    bool Keyboard::IsKeyDown(Key key)
    {
        return s_keyMap[key] == GLFW_REPEAT || s_keyMap[key] == GLFW_PRESS;
    }

    Vector2 Mouse::GetPosition()
    {
        return s_mousePos;
    }

    bool Mouse::ButtonDown(Button button)
    {
        return s_mouseButtonMap[button] == GLFW_PRESS;
    }

    double Mouse::GetScrollValue()
    {
        auto r = s_mouseScrollValue;
        s_mouseScrollValue = 0;
        return r;
    }
}