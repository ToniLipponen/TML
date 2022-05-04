#pragma once
#include <cstdint>
#include <TML/Export.h>

namespace tml
{
    struct TML_API Color
    {
        Color() = default;
        constexpr Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), a(255) {}
        constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
        explicit constexpr Color(uint32_t hex) noexcept
        : r((hex & 0xff000000) >> 24),
          g((hex & 0x00ff0000) >> 16),
          b((hex & 0x0000ff00) >> 8),
          a((hex & 0x000000ff))
          {

          }

        constexpr Color(int32_t hex) noexcept
        : r((hex & 0xff000000) >> 24),
          g((hex & 0x00ff0000) >> 16),
          b((hex & 0x0000ff00) >> 8),
          a((hex & 0x000000ff))
          {

          }

        inline constexpr Color operator/(float rhs) const noexcept {
            return Color(r / rhs, g / rhs, b / rhs, a / rhs);
        }

        inline constexpr Color operator*(float rhs) const noexcept {
            return Color(r * rhs, g * rhs, b * rhs, a * rhs);
        }

        inline constexpr Color operator+(const Color &color) const noexcept {
            return Color(r + color.r, g + color.g, b + color.b, a + color.a);
        }

        inline constexpr Color operator-(const Color& color) const noexcept {
            return Color(r - color.r, g - color.g, b - color.b, a - color.a);
        }
        inline constexpr uint32_t Hex() const noexcept {
            return (static_cast<uint32_t>(r) << 24) | (static_cast<uint32_t>(g) << 16) | (static_cast<uint32_t>(b) << 8) | static_cast<uint32_t>(a);
        }

        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 0;

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

    inline constexpr Color Color::Red         = Color(0xff0000ff);
    inline constexpr Color Color::Green       = Color(0x00ff00ff);
    inline constexpr Color Color::Blue        = Color(0x0000ffff);
    inline constexpr Color Color::Yellow      = Color(0xffff00ff);
    inline constexpr Color Color::Magenta     = Color(0xff00ffff);
    inline constexpr Color Color::Cyan        = Color(0x00ffffff);
    inline constexpr Color Color::Black       = Color(0x000000ff);
    inline constexpr Color Color::White       = Color(0xffffffff);
    inline constexpr Color Color::Transparent = Color(0x00000000);
}
