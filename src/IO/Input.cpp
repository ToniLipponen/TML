#include <TML/IO/Input.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <map>
#include <iostream>
// Keycode and action

static tml::ui32 KEYBOARD_LAST_KEY = -1;
static tml::i8 KEYBOARD_CHAR = 0;
static std::map<tml::i32, tml::i32> KEYS_MAP;

static std::map<tml::i32, tml::i32> MOUSE_BUTTON_MAP;
static std::string s_string;
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
    s_string.push_back(char(code));
    KEYBOARD_CHAR = char(code);
}

extern "C" void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_BACKSPACE && !s_string.empty())
        s_string.pop_back();
    KEYBOARD_LAST_KEY = key;
    KEYS_MAP[key] = action;
}

extern "C" void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    MOUSE_BUTTON_MAP[button] = action;
}

namespace tml {
    void Keyboard::BeginString()
    {
        s_string.clear();
    }
    std::string Keyboard::EndString()
    {
        std::string s = s_string;
        s_string.clear();
        return s;
    }
    const std::string& Keyboard::GetString()
    {
        return s_string;
    }

    i8 Keyboard::GetChar()
    {
        auto c = KEYBOARD_CHAR;
        return c;
    }
    ui32 Keyboard::GetKey()
    {
        auto key = KEYBOARD_LAST_KEY;
        KEYBOARD_LAST_KEY = -1;
        return key;
    }
    bool Keyboard::IsKeyPressed(Key key)
    {
        bool state = KEYS_MAP[key] == GLFW_PRESS;
        if(state)
            KEYS_MAP[key] = GLFW_RELEASE;
        return state;
    }

    bool Keyboard::IsKeyDown(Key key)
    {
        return KEYS_MAP[key] == GLFW_REPEAT || KEYS_MAP[key] == GLFW_PRESS;
    }

    Vector2 Mouse::GetPosition()
    {
        return s_mousePos;
    }

    bool Mouse::ButtonDown(Button button)
    {
        return MOUSE_BUTTON_MAP[button] == GLFW_PRESS;
    }

    double Mouse::GetScrollValue()
    {
        return s_mouseScrollValue;
    }
};