#pragma once
#include <TML/Export.h>
#include <string>
#include <vector>
#include <optional>

namespace tml::File
{
    [[nodiscard]] TML_API bool Exists(const std::string& filename) noexcept;
    [[nodiscard]] TML_API uint64_t Size(const std::string& filename) noexcept;

    /// Returns the file as raw bytes.
    [[nodiscard]] TML_API std::optional<std::vector<char>> GetBytes(const std::string& filename) noexcept;

    /// Returns the contents of a file as a single string.
    [[nodiscard]] TML_API std::optional<std::string> GetString(const std::string& filename) noexcept;

    /// Returns the contents of a file as a vector of strings, where each item in the vector is a line.
    [[nodiscard]] TML_API std::optional<std::vector<std::string>> GetLines(const std::string& filename) noexcept;
}
