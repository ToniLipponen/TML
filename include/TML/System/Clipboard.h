#pragma once
#include <TML/System/String.h>
#include <TML/System/Image.h>
#include <TML/Export.h>
#include <optional>

namespace tml::Clipboard
{
    [[nodiscard]] TML_API bool IsEmpty();
    TML_API void Clear();
    TML_API bool GetString(String& string);
    [[nodiscard]] TML_API std::optional<String> GetString();
    TML_API void SetString(const String& string);
}