#pragma once
#include <TML/System/String.h>
#include <TML/System/Image.h>
#include <TML/Export.h>

namespace tml
{
    namespace Clipboard
    {
        TML_API bool IsEmpty();
        TML_API bool HasImage();
        TML_API bool HasText();

        TML_API void Clear();
        TML_API bool GetString(String& string);
        TML_API bool GetImage(Image& image);

        TML_API void SetString(const String& string);
        TML_API void SetImage(const Image& image);
    }
}