#pragma once
#include <string>
#include <locale>
#include <codecvt>
#include <exception>

namespace tml
{
    namespace Util
    {
        inline std::string WstringToString(const std::wstring& str) noexcept
        {
            using convert_type = std::codecvt_utf8<wchar_t>;
            std::wstring_convert<convert_type, wchar_t> converter;
            return converter.to_bytes(str);
        }

        inline std::wstring StringToWstring(const std::string& str) noexcept
        {
            using convert_type = std::codecvt_utf8<wchar_t>;
            std::wstring_convert<convert_type, wchar_t> converter;
            return converter.from_bytes(str);
        }

        /** @brief Converts std::string to a given literal type.
         * @throws std::runtime_error if convertable type is not a literal type.
         */
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
                    throw std::invalid_argument("Convertable type is not literal.");
            }
            else
                throw std::invalid_argument("Convertable type is not literal.");
        }
    }
}