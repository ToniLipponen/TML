#include <TML/Window/Gamepad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace tml
{
    Gamepad::Gamepad()
    : m_id(0)
    {

    }

    Gamepad::Gamepad(int32_t id)
    : m_id(id)
    {

    }

    Vector2f Gamepad::GetAxisValue(Joystick joystick) noexcept
    {
        GLFWgamepadstate state{};
        glfwGetGamepadState(m_id, &state);

        if(joystick == Joystick::Left)
        {
            return { state.axes[GLFW_GAMEPAD_AXIS_LEFT_X], state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] };
        }
            
        return { state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] };
    }

    float Gamepad::GetTriggerValue(Trigger trigger) noexcept
    {
        GLFWgamepadstate state{};
        glfwGetGamepadState(m_id, &state);

        if(trigger == Trigger::Left)
        {
            /// Converting range from [-1.0 ... 1.0] to [0.0 ... 1.0], where 1.0 means the trigger is completely pressed down.
            return (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] + 1.0f) / 2.0f;
        }

        return (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.0f) / 2.0f;
    }

    bool Gamepad::IsButtonPressed(Gamepad::Button button) noexcept
    {
        GLFWgamepadstate state{};
        glfwGetGamepadState(m_id, &state);

        const auto returnValue = (state.buttons[button] == (GLFW_PRESS & m_buttonStates[button])) != GLFW_PRESS;
        m_buttonStates[button] = state.buttons[button];

        return returnValue;
    }

    bool Gamepad::IsButtonDown(Gamepad::Button button) noexcept
    {
        GLFWgamepadstate state{};
        glfwGetGamepadState(m_id, &state);
        return state.buttons[button] == GLFW_PRESS;
    }

    bool Gamepad::IsConnected() noexcept
    {
        return glfwJoystickPresent(m_id) && glfwJoystickIsGamepad(m_id);
    }

    int32_t Gamepad::GetId() noexcept
    {
        return m_id;
    }
}