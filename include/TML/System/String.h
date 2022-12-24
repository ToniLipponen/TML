#pragma once
#include <string>
#include "String/tinyutf8.h"

namespace tml
{
    using String = tml::string;

    template<typename T>
    [[maybe_unused, nodiscard]] inline constexpr T StringToType(const std::string& str) noexcept
    {
        if constexpr(!std::is_class<T>::value)
        {
            if constexpr(std::is_integral<T>::value)
            {
                if constexpr(std::is_same<T,bool>::value)
                {
                    return str == "true";
                }
                else if constexpr(std::is_unsigned<T>::value)
                {
                    return std::stoull(str);
                }

                return std::stoll(str);
            }
            else if constexpr(std::is_floating_point<T>::value)
            {
                return std::stod(str);
            }

            static_assert(true, "Convertable type is not literal.");
        }

        static_assert(true, "Convertable type is not literal.");
    }
}