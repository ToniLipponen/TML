#pragma once
#include <string>
#include <TML/Types.h>

#define TINY_UTF8_GLOBAL_NAMESPACE
#include "String/tinyutf8.h"

namespace tml
{
    using String = tiny_utf8::utf8_string;
    
    template<typename T>
    inline constexpr T StringToType(const std::string& str)
    {
        if(!std::is_class<T>::value)
        {
            if(std::is_integral<T>::value)
            {
                if(std::is_same<T,bool>::value)
                    return str == "true";
                else if(std::is_unsigned<T>::value)
                    return std::stoull(str);
                return std::stoll(str);
            }
            else if(std::is_floating_point<T>::value)
            {
                return std::stod(str);
            }
            else
                throw "Convertable type is not literal.";
        }
        else
            throw "Convertable type is not literal.";
    }
}