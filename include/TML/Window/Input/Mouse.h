#pragma once
#include <TML/System/Math/Vector2.h>

namespace tml::Mouse
{
    enum Button
    {
        Left,
        Right,
        Middle,
        Four,
        Five,
        Six,
        Seven,
        Last
    };

    [[nodiscard]] TML_API Vector2d GetPosition() noexcept;
    [[nodiscard]] TML_API bool ButtonDown(Button button) noexcept;
}