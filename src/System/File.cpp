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

    std::vector<char> ReadBytes(const std::string& filename) noexcept
    {
        if(Exists(filename))
        {
            std::ifstream file(filename, std::ios::binary);
            const auto fileSize = Size(filename);

            std::vector<char> data(fileSize, 0);
            file.read(&data[0], static_cast<std::streamsize>(fileSize));

            return data;
        }

        return {};
    }

    std::string ReadString(const std::string& filename) noexcept
    {
        if(Exists(filename))
        {
            std::string lines, line;
            std::ifstream file(filename);

            while(std::getline(file, line))
            {
                lines.append(line + "\n");
            }

            return lines;
        }

        return {};
    }

    std::vector<std::string> ReadLines(const std::string& filename) noexcept
    {
        if(Exists(filename))
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

        return {};
    }

    bool WriteBytes(const std::vector<char>& bytes, const std::string& filename) noexcept
    {
        if(!filename.empty())
        {
            std::ofstream file(filename);
            file.write(bytes.data(), bytes.size());

            return true;
        }

        return false;
    }
}
