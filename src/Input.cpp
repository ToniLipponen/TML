#include <TML/Input.h>
#include <TML/Utilities/Utilities.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <map>
#include <iostream>
// Keycode and action

static tml::i32 KEYBOARD_LAST_KEY = -1;
static tml::ui32 KEYBOARD_CHAR = 0;
static std::map<tml::i32, tml::i32> KEYS_MAP;
static std::map<tml::i32, tml::i32> MOUSE_BUTTON_MAP;
static std::string s_string;
#define MOUSE_CLICK 3

static void char_callback(GLFWwindow* window, unsigned int code)
{
    s_string.push_back(char(code));
    KEYBOARD_CHAR = code;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_BACKSPACE && !s_string.empty())
        s_string.pop_back();
    KEYBOARD_LAST_KEY = key;
    KEYS_MAP[key] = action;
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(MOUSE_BUTTON_MAP[button] == GLFW_PRESS && action == GLFW_PRESS)
        MOUSE_BUTTON_MAP[button] = MOUSE_CLICK;
    else
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
    const std::string& Keyboard::GetString() {
        return s_string;
    }
    bool Keyboard::IsKeyPressed(Key key) {
        bool state = KEYS_MAP[key] == GLFW_PRESS;
        if(state)
            KEYS_MAP[key] = GLFW_RELEASE;
        return state;
    }

    bool Keyboard::IsKeyDown(Key key) {
        return KEYS_MAP[key] == GLFW_REPEAT || KEYS_MAP[key] == GLFW_PRESS;
    }

    void Keyboard::Initialize()
    {
        glfwSetInputMode(glfwGetCurrentContext(), GLFW_STICKY_KEYS, GLFW_TRUE);
        glfwSetCharCallback(glfwGetCurrentContext(), char_callback);
        glfwSetKeyCallback(glfwGetCurrentContext(), key_callback);
        glfwSetMouseButtonCallback(glfwGetCurrentContext(), mouse_button_callback);
    }

    ui32 Keyboard::GetChar()
    {
        auto c = KEYBOARD_CHAR;
        KEYBOARD_CHAR = 0;
        return c;
    }

    Vector2 Mouse::GetPosition() {
        double x = 0, y = 0;
        glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
        return {static_cast<float>(x), static_cast<float>(y)};
    }

    bool Mouse::ButtonClicked(Button button) {
        bool state = MOUSE_BUTTON_MAP[button] == GLFW_PRESS;
        if(state)
            MOUSE_BUTTON_MAP[button] = GLFW_RELEASE;
        return state;
    }

    bool Mouse::ButtonDown(Button button)
    {
        return MOUSE_BUTTON_MAP[button] = GLFW_PRESS;
    }
};