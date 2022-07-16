#pragma once
#include <TML/Export.h>
#include <string>
#include <vector>

namespace tml::File
{
    TML_API bool Exists(const std::string& filename) noexcept;

    TML_API uint64_t Size(const std::string& filename) noexcept;

    TML_API std::vector<char> GetBytes(const std::string& filename) noexcept;

    TML_API std::string GetString(const std::string& filename) noexcept;

    TML_API std::vector<std::string> GetLines(const std::string& filename) noexcept;
}