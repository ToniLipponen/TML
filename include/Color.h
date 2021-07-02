#pragma once
#include "Types.h"


struct Color
{
    Color() = default;
    constexpr Color(float r, float g, float b) : r(r),g(g),b(b),a(255.f){}
    constexpr Color(float r, float g, float b, float a) : r(r),g(g),b(b),a(a){}
    constexpr Color(ui32 hex)
    {
        r = static_cast<float>((hex & 0xff000000) >> 24);
        g = static_cast<float>((hex & 0x00ff0000) >> 16);
        b = static_cast<float>((hex & 0x0000ff00) >> 8);
        a = static_cast<float>((hex & 0x000000ff));
    }
    constexpr Color operator/(const float rhs) const noexcept
    {
        const float x = 1.0f / rhs;
        return Color(r*x, g*x, b*x, a*x);
    }
    constexpr Color operator*(const float rhs) const noexcept
    {
        return Color(r*rhs, g*rhs, b*rhs, a*rhs);
    }
    float r = 0;
    float g = 0;
    float b = 0;
    float a = 0;
};