#pragma once
#include <TML/Types.h>

namespace tml
{
    struct Color
    {
        Color() = default;
        constexpr Color(ui8 r, ui8 g, ui8 b) : r(r), g(g), b(b), a(255.f) {}
        constexpr Color(ui8 r, ui8 g, ui8 b, ui8 a) : r(r), g(g), b(b), a(a) {}
        explicit constexpr Color(ui32 hex) noexcept
        : r((hex & 0xff000000) >> 24),
          g((hex & 0x00ff0000) >> 16),
          b((hex & 0x0000ff00) >> 8),
          a((hex & 0x000000ff))
          {

          }

        constexpr Color(i32 hex) noexcept
        : r((hex & 0xff000000) >> 24),
          g((hex & 0x00ff0000) >> 16),
          b((hex & 0x0000ff00) >> 8),
          a((hex & 0x000000ff))
          {

          }

        inline constexpr Color operator/(i32 rhs) const noexcept {
            return Color(r / rhs, g / rhs, b / rhs, a / rhs);
        }

        inline constexpr Color operator*(i32 rhs) const noexcept {
            return Color(r * rhs, g * rhs, b * rhs, a * rhs);
        }

        inline constexpr Color operator+(const Color &color) const noexcept {
            return Color(r + color.r, g + color.g, b + color.b, a + color.a);
        }

        inline constexpr Color operator-(const Color& color) const noexcept {
            return Color(r - color.r, g - color.g, b - color.b, a - color.a);
        }
        inline constexpr ui32 Hex() const noexcept {
            return (static_cast<ui32>(r) << 24) | (static_cast<ui32>(g) << 16) | (static_cast<ui32>(b) << 8) | static_cast<ui32>(a);
        }

        ui8 r = 0;
        ui8 g = 0;
        ui8 b = 0;
        ui8 a = 0;

        static const Color Red;
        static const Color Green;
        static const Color Blue;

        static const Color Yellow;
        static const Color Magenta;
        static const Color Cyan;

        static const Color Black;
        static const Color White;
        static const Color Transparent;
    };
}

