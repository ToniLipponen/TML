#include <TML/Window/Input.h>
#include <TML/Window/Event.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

using tml::Event;

namespace tml
{
    bool Keyboard::IsKeyPressed(Key key)
    {
        auto context = glfwGetCurrentContext();
        if(!context)
            return false;

        return glfwGetKey(context, key) == GLFW_PRESS;
    }

    bool Keyboard::IsKeyDown(Key key)
    {
        auto context = glfwGetCurrentContext();
        if(!context)
            return false;

        auto state = glfwGetKey(context, key);
        return state == GLFW_REPEAT || state == GLFW_PRESS;
    }

    Vector2d Mouse::GetPosition()
    {
        auto context = glfwGetCurrentContext();
        if(!context)
            return {0,0};

        Vector2d mousePos;
        glfwGetCursorPos(context, &mousePos.x, &mousePos.y);
        return mousePos;
    }

    bool Mouse::ButtonDown(Button button)
    {
        auto context = glfwGetCurrentContext();
        if(!context)
            return false;
        return glfwGetMouseButton(context, button) == GLFW_PRESS;
    }
}