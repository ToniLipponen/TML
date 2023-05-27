#include <TML/System/File.h>
#include <fstream>
#include <filesystem>

namespace tml::File
{
    bool Exists(const std::string& filename) noexcept
    {
        return std::filesystem::exists(filename);
    }

    uint64_t Size(const std::string& filename) noexcept
    {
        return std::filesystem::file_size(filename);
    }

    std::optional<std::vector<char>> GetBytes(const std::string& filename) noexcept
    {
        if(std::filesystem::exists(filename))
        {
            std::ifstream file(filename, std::ios::binary);
            const auto fileSize = Size(filename);

            std::vector<char> data(fileSize, 0);
            file.read(&data[0], static_cast<std::streamsize>(fileSize));

            return data;
        }

        return std::nullopt;
    }

    std::optional<std::string> GetString(const std::string& filename) noexcept
    {
        if(std::filesystem::exists(filename))
        {
            std::string lines, line;
            std::ifstream file(filename);

            while(std::getline(file, line))
            {
                lines.append("\n" + line);
            }

            return lines;
        }

        return std::nullopt;
    }

    std::optional<std::vector<std::string>> GetLines(const std::string& filename) noexcept
    {
        if(std::filesystem::exists(filename))
        {
            std::vector<std::string> lines;
            std::string line;
            std::ifstream file(filename);

            while(std::getline(file, line))
            {
                lines.push_back(line);
            }

            return lines;
        }

        return std::nullopt;
    }
}
