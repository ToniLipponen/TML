#include <TML/System/Input.h>
#include <TML/System/Event.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using tml::Event;

namespace tml
{
    bool Keyboard::IsKeyPressed(Key key)
    {
        return glfwGetKey(glfwGetCurrentContext(), key) == GLFW_PRESS;
    }

    bool Keyboard::IsKeyDown(Key key)
    {
        auto state = glfwGetKey(glfwGetCurrentContext(), key);
        return state == GLFW_REPEAT || state == GLFW_PRESS;
    }

    Vector2d Mouse::GetPosition()
    {
        Vector2d mousePos;
        glfwGetCursorPos(glfwGetCurrentContext(), &mousePos.x, &mousePos.y);
        return mousePos;
    }

    bool Mouse::ButtonDown(Button button)
    {
        return glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_PRESS;
    }

}