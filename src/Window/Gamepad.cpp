#include <TML/Window/Gamepad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace tml
{
    namespace Gamepad
    {
        static unsigned char s_buttonStates[GamepadCount][ButtonCount]{};

        bool IsGamepadConnected(GamepadEnum gamepad)
        {
            return static_cast<bool>(glfwJoystickPresent(gamepad)) && static_cast<bool>(glfwJoystickIsGamepad(gamepad));
        }

        Vector2f GetAxisValue(GamepadEnum gamepad, Joystick joystick)
        {
            GLFWgamepadstate state{};
            glfwGetGamepadState(gamepad, &state);

            if(joystick == LeftJoystick)
                return {state.axes[GLFW_GAMEPAD_AXIS_LEFT_X], state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]};
            else
                return {state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]};
        }

        float GetTriggerValue(GamepadEnum gamepad, Trigger trigger)
        {
            GLFWgamepadstate state{};
            glfwGetGamepadState(gamepad, &state);

            if(trigger == LeftTrigger)
                return (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] + 1.0f) / 2.0f; //!< Converting range from [-1.0 ... 1.0] to [0.0 ... 1.0], where 1.0 means the trigger is completely pressed down.
            else
                return (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.0f) / 2.0f;
        }

        bool IsButtonPressed(GamepadEnum gamepad, Gamepad::Button button)
        {
            GLFWgamepadstate state{};
            glfwGetGamepadState(gamepad, &state);

            const auto returnValue = state.buttons[button] == (GLFW_PRESS & s_buttonStates[gamepad][button]) != GLFW_PRESS;
            s_buttonStates[gamepad][button] = state.buttons[button];
            return returnValue;
        }

        bool IsButtonDown(GamepadEnum gamepad, Gamepad::Button button)
        {
            GLFWgamepadstate state{};
            glfwGetGamepadState(gamepad, &state);
            return state.buttons[button] == GLFW_PRESS;
        }
    }
}