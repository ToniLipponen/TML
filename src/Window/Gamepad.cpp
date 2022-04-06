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
            return static_cast<bool>(glfwJoystickPresent(gamepad));
        }

        Vector2f GetAxis(GamepadEnum gamepad, Joystick joystick)
        {
            int axisCount = 0;
            const auto* floats = glfwGetJoystickAxes(gamepad, &axisCount);
            const auto* axis = reinterpret_cast<const Vector2f*>(floats);

            if(floats == nullptr || axisCount == 0)
                return {};

            if(joystick == LeftJoystick)
                return axis[0];

            else
                return {floats[3], floats[4] };
        }

        bool IsButtonPressed(GamepadEnum gamepad, Gamepad::Button button)
        {
            int buttonCount = 0;
            const auto* buttons = glfwGetJoystickButtons(gamepad, &buttonCount);

            if(buttonCount && buttons && button <= buttonCount)
            {
                const auto returnValue = buttons[button] == GLFW_PRESS && s_buttonStates[gamepad][button] != GLFW_PRESS;
                s_buttonStates[gamepad][button] = buttons[button];
                return returnValue;
            }

            return false;
        }

        bool IsButtonDown(GamepadEnum gamepad, Gamepad::Button button)
        {
            int buttonCount = 0;
            const auto* buttons = glfwGetJoystickButtons(gamepad, &buttonCount);

            if(buttonCount && buttons && button <= buttonCount)
            {
                return buttons[button] == GLFW_PRESS;
            }

            return false;
        }
    }
}