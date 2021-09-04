#include "../include/Input.h"
#define GLFW_INCLUDE_NONE
#include "../external-headers//GLFW/glfw3.h"
#include <map>

// Keycode and action
static std::map<tml::i32, tml::i32> KEYS_MAP;
static std::map<tml::i32, tml::i32> MOUSE_BUTTON_MAP;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    KEYS_MAP[key] = action;
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    MOUSE_BUTTON_MAP[button] = action;
}

namespace tml {
    bool Keyboard::IsKeyPressed(Key key) {
        return KEYS_MAP[key] == GLFW_PRESS;
    }

    bool Keyboard::IsKeyDown(Key key) {
        return KEYS_MAP[key] == GLFW_REPEAT || KEYS_MAP[key] == GLFW_PRESS;
    }

    void Keyboard::Initialize() {
        glfwSetKeyCallback(glfwGetCurrentContext(), key_callback);
        glfwSetMouseButtonCallback(glfwGetCurrentContext(), mouse_button_callback);
    }

    Vector2 Mouse::GetPosition() {
        double x = 0, y = 0;
        glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
        return Vector2(x, y);
    }

    bool Mouse::ButtonPressed(Button button) {
        return MOUSE_BUTTON_MAP[button] == GLFW_PRESS;
    }
};