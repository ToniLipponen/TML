#include <TML/Window/Gamepad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace tml::Gamepad
{
    static unsigned char s_buttonStates[GamepadCount][ButtonCount]{};

    bool IsGamepadConnected(GamepadEnum gamepad) noexcept
    {
        return static_cast<bool>(glfwJoystickPresent(gamepad)) && static_cast<bool>(glfwJoystickIsGamepad(gamepad));
    }

    Vector2f GetAxisValue(GamepadEnum gamepad, Joystick joystick) noexcept
    {
        GLFWgamepadstate state{};
        glfwGetGamepadState(gamepad, &state);

        if(joystick == Joystick::Left)
        {
            return {state.axes[GLFW_GAMEPAD_AXIS_LEFT_X], state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]};
        }

        return {state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]};
    }

    float GetTriggerValue(GamepadEnum gamepad, Trigger trigger) noexcept
    {
        GLFWgamepadstate state{};
        glfwGetGamepadState(gamepad, &state);

        if(trigger == Trigger::Left)
        {
            /// Converting range from [-1.0 ... 1.0] to [0.0 ... 1.0], where 1.0 means the trigger is completely pressed down.
            return (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] + 1.0f) / 2.0f;
        }

        return (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.0f) / 2.0f;
    }

    bool IsButtonPressed(GamepadEnum gamepad, Gamepad::Button button) noexcept
    {
        GLFWgamepadstate state{};
        glfwGetGamepadState(gamepad, &state);

        const auto returnValue = (state.buttons[button] == (GLFW_PRESS & s_buttonStates[gamepad][button])) != GLFW_PRESS;
        s_buttonStates[gamepad][button] = state.buttons[button];
        return returnValue;
    }

    bool IsButtonDown(GamepadEnum gamepad, Gamepad::Button button) noexcept
    {
        GLFWgamepadstate state{};
        glfwGetGamepadState(gamepad, &state);
        return state.buttons[button] == GLFW_PRESS;
    }
}