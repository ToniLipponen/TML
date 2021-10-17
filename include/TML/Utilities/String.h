#pragma once
#include <string>
#include <locale>
#include <codecvt>

namespace tml::Util
{
    inline std::string wstringToString(const std::wstring& str) noexcept
    {
        using convert_type = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_type, wchar_t> converter;
        return converter.to_bytes(str);
    }

    inline std::wstring stringToWstring(const std::string& str) noexcept
    {
        using convert_type = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_type, wchar_t> converter;
        return converter.from_bytes(str);
    }
}