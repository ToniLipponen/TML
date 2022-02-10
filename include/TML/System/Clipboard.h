#pragma once
#include <TML/System/String.h>
#include "Image.h"

namespace tml
{
    namespace Clipboard
    {
        bool IsEmpty();
        bool HasImage();
        bool HasText();

        void Clear();
        bool GetString(String& string);
        bool GetImage(Image& image);

        void SetString(const String& string);
        void SetImage(const Image& image);
    }
}