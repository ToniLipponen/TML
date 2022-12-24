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

    [[maybe_unused, nodiscard]] TML_API Vector2d GetPosition() noexcept;
    [[maybe_unused, nodiscard]] TML_API bool ButtonDown(Button button) noexcept;
}