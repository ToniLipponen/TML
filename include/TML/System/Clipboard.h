#pragma once
#include <TML/System/String.h>
#include <TML/System/Image.h>
#include <TML/Export.h>

namespace tml::Clipboard
{
    [[maybe_unused, nodiscard]] TML_API bool IsEmpty();
    [[maybe_unused, nodiscard]] TML_API bool HasImage();
    [[maybe_unused, nodiscard]] TML_API bool HasText();
    [[maybe_unused]]            TML_API void Clear();
    [[maybe_unused, nodiscard]] TML_API bool GetString(String& string);
    [[maybe_unused, nodiscard]] TML_API bool GetImage(Image& image);
    [[maybe_unused]]            TML_API void SetString(const String& string);
}