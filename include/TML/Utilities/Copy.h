#pragma once

namespace tml
{
    namespace Util
    {
        template<typename T>
        inline constexpr T* Copy(T* dest, const T* source, unsigned int elements) noexcept
        {
            for(auto i = 0; i < elements; ++i)
                dest[i] = source[i];
            return dest;
        }
    }
}