#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

namespace tml::File
{
    inline bool Exists(const std::string& filename) noexcept
    {
        return std::filesystem::exists(filename);
    }

    inline std::vector<char> GetBytes(const std::string& filename) noexcept
    {
        if(std::filesystem::exists(filename))
        {
            std::ifstream file(filename, std::ios::binary | std::ios::ate);
            const auto fileLength = file.tellg();
            file.seekg(std::ios::beg);

            std::vector<char> data(fileLength, 0);
            file.read(&data[0], fileLength);

            return data;
        }

        return {};
    }

    inline bool GetBytes(const std::string& filename, std::vector<char>& output) noexcept
    {
        if(std::filesystem::exists(filename))
        {
            std::ifstream file(filename, std::ios::binary | std::ios::ate);
            const auto fileLength = file.tellg();
            file.seekg(std::ios::beg);

            std::vector<char> data(fileLength, 0);
            file.read(&data[0], fileLength);
            output = std::move(data);

            return true;
        }

        return false;
    }

    inline std::string GetString(const std::string& filename) noexcept
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

        return "";
    }

    inline bool GetString(const std::string& filename, std::string& output) noexcept
    {
        if(std::filesystem::exists(filename))
        {
            std::string line;
            std::ifstream file(filename);

            while(std::getline(file, line))
            {
                output.append("\n" + line);
            }

            return true;
        }

        return false;
    }

    inline std::vector<std::string> GetLines(const std::string& filename) noexcept
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

        return {};
    }

    inline bool GetLines(const std::string& filename, std::vector<std::string>& output) noexcept
    {
        if(std::filesystem::exists(filename))
        {
            std::string line;
            std::ifstream file(filename);

            while(std::getline(file, line))
            {
                output.push_back(line);
            }

            return true;
        }

        return false;
    }
}