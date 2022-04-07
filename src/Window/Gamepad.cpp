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

        Vector2f GetAxisValue(GamepadEnum gamepad, Joystick joystick)
        {
            int axisCount = 0;
            const auto* axis = glfwGetJoystickAxes(gamepad, &axisCount);

            if(axis == nullptr || axisCount == 0)
                return {};


            /// Note to self. Check out why this is works and the code below it doesn't.
            if(joystick == LeftJoystick)
                return {axis[GLFW_GAMEPAD_AXIS_LEFT_X], axis[GLFW_GAMEPAD_AXIS_LEFT_Y]};
            else
                return {axis[GLFW_GAMEPAD_AXIS_RIGHT_Y], axis[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER]};


            /// This doesn't work correctly on my system.
            if(joystick == LeftJoystick)
                return {axis[GLFW_GAMEPAD_AXIS_LEFT_X], axis[GLFW_GAMEPAD_AXIS_LEFT_Y]};
            else
                return {axis[GLFW_GAMEPAD_AXIS_RIGHT_X], axis[GLFW_GAMEPAD_AXIS_RIGHT_Y]};
        }

        float GetTriggerValue(GamepadEnum gamepad, Trigger trigger)
        {
            int axisCount = 0;
            const auto* axis = glfwGetJoystickAxes(gamepad, &axisCount);

            if(axis && axisCount && axisCount >= 4)
            {

                if(trigger == LeftTrigger)
                    return (axis[GLFW_GAMEPAD_AXIS_RIGHT_X] + 1.0f) / 2.0f;
                else
                    return (axis[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.0f) / 2.0f;

                /// Same problem as on line 26
                if(trigger == LeftTrigger)
                    return (axis[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] + 1.0f) / 2.0f;
                else
                    return (axis[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.0f) / 2.0f;
            }

            return 0;
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