#pragma once
#include <TML/System/Math/Vector2.h>

namespace tml
{
    namespace Gamepad
    {
        enum GamepadEnum
        {
            Gamepad0,
            Gamepad1,
            Gamepad2,
            Gamepad3,
            Gamepad4,
            Gamepad5,
            Gamepad6,
            Gamepad7,
            Gamepad8,
            Gamepad9,
            Gamepad10,
            Gamepad11,
            Gamepad12,
            Gamepad13,
            Gamepad14,
            Gamepad15,
            GamepadCount
        };

        enum Button
        {
            Button0,
            Button1,
            Button2,
            Button3,
            Button4,
            Button5,
            Button6,
            Button7,
            Button8,
            Button9,
            Button10,
            Button11,
            Button12,
            Button13,
            Button14,
            Button15,
            ButtonCount
        };

        enum Joystick { LeftJoystick, RightJoystick };

        bool IsGamepadConnected(GamepadEnum gamepad);
        Vector2f GetAxis(GamepadEnum gamepad, Joystick joystick);
        bool IsButtonPressed(GamepadEnum gamepad, Gamepad::Button button);
        bool IsButtonDown(GamepadEnum gamepad, Gamepad::Button button);
    }
}