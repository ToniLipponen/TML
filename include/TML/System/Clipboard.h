#pragma once
#include <TML/System/String.h>
#include <TML/System/Image.h>
#include <TML/Export.h>
#include <optional>

namespace tml::Clipboard
{
    [[nodiscard]] TML_API bool IsEmpty();
    [[nodiscard]] TML_API bool HasImage();
    [[nodiscard]] TML_API bool HasText();
    TML_API void Clear();
    TML_API bool GetString(String& string);
    TML_API bool GetImage(Image& image);
    [[nodiscard]] TML_API std::optional<String> GetString();
    [[nodiscard]] TML_API std::optional<Image> GetImage();
    TML_API void SetString(const String& string);
}