#pragma once
#include <TML/Export.h>
#include <string>
#include <vector>
#include <optional>

namespace tml::File
{
    /// Returns true if file exists
    TML_API bool Exists(const std::string& filename) noexcept;

    /// Returns the size of a file in bytes
    TML_API uint64_t Size(const std::string& filename) noexcept;

    /// Returns the file as raw bytes.
    TML_API std::vector<char> ReadBytes(const std::string& filename) noexcept;

    /// Returns the contents of a file as a single string.
    TML_API std::string ReadString(const std::string& filename) noexcept;

    /// Returns the contents of a file as a vector of strings, where each item in the vector is a line.
    TML_API std::vector<std::string> ReadLines(const std::string& filename) noexcept;

    /// Write bytes to file
    TML_API bool WriteBytes(const std::vector<char>& bytes, const std::string& filename) noexcept;
}
