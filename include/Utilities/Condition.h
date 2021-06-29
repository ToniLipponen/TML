#pragma once

namespace tl
{
    template<typename T>
    inline constexpr T& condition(bool condition, const T& _true, const T& _false) noexcept
    {
        return condition ? _true : _false;
    }

    inline constexpr void* condition(bool condition, void* _true, void* _false) noexcept
    {
        return condition ? _true : _false;
    }
};