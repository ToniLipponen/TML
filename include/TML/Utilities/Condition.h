#pragma once

namespace tml
{
    template<typename T>
    inline constexpr const T& Condition(bool condition, const T& _true, const T& _false) noexcept
    {
        return condition ? _true : _false;
    }

    inline constexpr void* Condition(bool condition, void* _true, void* _false) noexcept
    {
        return condition ? _true : _false;
    }
}