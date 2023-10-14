#pragma once
#include <TML/System/EventHandler.h>
#include <TML/System/Math/Vector2.h>
#include <TML/Export.h>

namespace tml
{
    class TML_API Gamepad
    {
    public:
        enum Button
        {
            Button0             = 0,
            ButtonA             = 0,
            ButtonCross         = 0,

            Button1             = 1,
            ButtonB             = 1,
            ButtonCircle        = 1,

            Button2             = 2,
            ButtonX             = 2,
            ButtonSquare        = 2,

            Button3             = 3,
            ButtonY             = 3,
            ButtonTriangle      = 3,

            Button4             = 4,
            ButtonLeftBumper    = 4,
            ButtonL1            = 4,

            Button5             = 5,
            ButtonRightBumper   = 5,
            ButtonR1            = 5,

            Button6             = 6,
            ButtonBack          = 6,
            ButtonShare         = 6,

            Button7             = 7,
            ButtonMenu          = 7,

            Button8             = 8,
            ButtonXbox          = 8,
            ButtonPlayStation   = 8,

            Button9             = 9,
            LeftStick           = 9,

            Button10            = 10,
            RightStick          = 10,

            Button11            = 11,
            ButtonUp            = 11,

            Button12            = 12,
            ButtonRight         = 12,

            Button13            = 13,
            ButtonDown          = 13,

            Button14            = 14,
            ButtonLeft          = 14,
            ButtonCount
        };

        enum class Joystick { Left, Right };
        enum class Trigger  { Left, Right };

    public:
        Gamepad();
        Gamepad(int32_t id);
        Vector2f GetAxisValue(Joystick joystick) noexcept;
        float GetTriggerValue(Trigger trigger) noexcept;
        bool IsButtonPressed(Button button) noexcept;
        bool IsButtonDown(Button button) noexcept;
        bool IsConnected() noexcept;
        int32_t GetId() noexcept;

        friend class GamepadManager;
    private:
        uint8_t m_buttonStates[ButtonCount];
        int32_t m_id = 0;
    };
}