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

    class DragAndDrop // TODO: Maybe change this at some point?
    {
    public:
        static bool IsEmpty();
        static void Clear(); // Clear list of dropped files.
        static const std::vector<std::string>& GetFiles();
    };
}