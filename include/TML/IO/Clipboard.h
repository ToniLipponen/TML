#pragma once
#include <string>
#include <vector>

namespace tml
{
    class Clipboard
    {
    public:
        static bool IsEmpty();
        static void Clear();
        static std::string GetString();
    };
}