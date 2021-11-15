#pragma once
#include "TML/Utilities/Types.h"

#define RED         Color(255,0,0,255)
#define GREEN       Color(0,255,0,255)
#define BLUE        Color(0,0,255,255)
#define WHITE       Color(255,255,255,255)
#define BLACK       Color(0,0,0,255)
#define TRANSPARENT Color(0,0,0,0)

namespace tml
{
    struct Color {
        Color() = default;
        constexpr Color(float r, float g, float b) : r(r), g(g), b(b), a(255.f) {}
        constexpr Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
        explicit constexpr Color(ui32 hex) noexcept
        : r(static_cast<float>((hex & 0xff000000) >> 24)),
          g(static_cast<float>((hex & 0x00ff0000) >> 16)),
          b(static_cast<float>((hex & 0x0000ff00) >> 8)),
          a(static_cast<float>((hex & 0x000000ff))){}
        constexpr Color(i32 hex) noexcept
        : r(static_cast<float>((hex & 0xff000000) >> 24)),
          g(static_cast<float>((hex & 0x00ff0000) >> 16)),
          b(static_cast<float>((hex & 0x0000ff00) >> 8)),
          a(static_cast<float>((hex & 0x000000ff))){}

        inline constexpr Color operator/(const float rhs) const noexcept {
            return {r / rhs, g / rhs, b / rhs, a / rhs};
        }

        inline constexpr Color operator*(const float rhs) const noexcept {
            return {r * rhs, g * rhs, b * rhs, a * rhs};
        }

        inline constexpr Color operator+(const Color &color) const noexcept {
            return {r + color.r, g + color.g, b + color.b, a + color.a};
        }
        inline ui32 Hex() const noexcept {
            ui32 h = (static_cast<ui32>(r) << 24);
            h += (static_cast<ui32>(g) << 16);
            h += (static_cast<ui32>(b) << 8);
            h += static_cast<ui32>(a);
            return h;
        }

        float r = 0;
        float g = 0;
        float b = 0;
        float a = 0;
    };
}