#pragma once

namespace tml
{
    template<typename T>
    inline constexpr const T& Condition(bool condition, const T& _true, const T& _false) noexcept
    {
        return condition ? _true : _false;
    }

    template<typename T>
    inline constexpr T* Condition(bool condition, T* _true, T* _false) noexcept
    {
        return condition ? _true : _false;
    }
}