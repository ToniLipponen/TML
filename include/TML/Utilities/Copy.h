#pragma once

namespace tl
{
    template<typename T>
    inline constexpr T* copy(T* dest, T* source, unsigned int elements) noexcept
    {
        for(auto i = 0; i < elements; ++i)
            dest[i] = source[i];
        return dest;
    }
};