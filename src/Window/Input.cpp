#include <TML/Window/Input.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace tml
{
    [[maybe_unused, nodiscard]] bool Keyboard::IsKeyPressed(Key key) noexcept
    {
        auto context = glfwGetCurrentContext();

        if(!context)
        {
            return false;
        }

        return glfwGetKey(context, static_cast<int>(key)) == GLFW_PRESS;
    }

    [[maybe_unused, nodiscard]] bool Keyboard::IsKeyDown(Key key) noexcept
    {
        auto context = glfwGetCurrentContext();

        if(!context)
        {
            return false;
        }

        auto state = glfwGetKey(context, static_cast<int>(key));
        return state == GLFW_REPEAT || state == GLFW_PRESS;
    }

    [[maybe_unused, nodiscard]] Vector2d Mouse::GetPosition() noexcept
    {
        auto context = glfwGetCurrentContext();

        if(!context)
        {
            return {0,0};
        }

        Vector2d mousePos;
        glfwGetCursorPos(context, &mousePos.x, &mousePos.y);
        return mousePos;
    }

    [[maybe_unused, nodiscard]] bool Mouse::ButtonDown(Button button) noexcept
    {
        auto context = glfwGetCurrentContext();

        if(!context)
        {
            return false;
        }

        return glfwGetMouseButton(context, static_cast<int>(button)) == GLFW_PRESS;
    }
}