#pragma once
#include <string>
#include <locale>
#include <codecvt>

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

        template<typename T>
        inline constexpr T StringToType(const std::string& str) noexcept
        {
            if(str.length() == 0 || str.length() > 10)
                return 0;
            if(std::is_integral<T>::value)
            {
                return std::stol(str);
            }
            else if(std::is_floating_point<T>::value)
            {
                return std::stod(str);
            }
            return std::stoi(str);
        }
    }
}