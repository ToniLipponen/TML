#pragma once
#include <TML/System/String.h>

namespace tml
{
    class Clipboard
    {
    public:
        static bool IsEmpty();
        static void Clear();
        static String GetString();
    };
}