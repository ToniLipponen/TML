#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Logger.h"

namespace tml
{
    class FileBase
    {
    public:
        FileBase() = default;
        explicit FileBase(const std::string& filename)
        {
//            m_stream.open(filename, std::ios::in | std::ios::out | std::ios::binary);
            m_stream.open(filename, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
            if (!m_stream.is_open() || m_stream.fail())
            {
                m_isValid = false;
                Logger::ErrorMessage("Could not open file -> %s", filename.c_str());
            }

            m_isValid = true;
            m_dataLen = m_stream.tellg();
            m_stream.seekg(0, std::iostream::beg);
        }

        virtual ~FileBase()
        {
            m_stream.close();
        }

        void Close()
        {
            m_stream.close();
        }

        virtual bool Open(const std::string& filename) = 0;

        static bool FileExists(const std::string& filename)
        {
            std::ifstream file(filename);
            const bool exists = file.is_open();
            file.close();
            return exists;
        }

        ui32 FileSize() const noexcept
        {
            return m_dataLen;
        }

        static i64 FileSize(const std::string &filename)
        {
            std::ifstream file(filename);
            if (!file.is_open() || file.fail())
                return 0;

            file.seekg(0, std::iostream::end);
            i64 size = file.tellg();
            file.close();
            return size;
        }
    protected:
        i64 m_dataLen = 0;
        bool m_isValid = false;
        std::fstream m_stream;
    };

    class InFile : public FileBase
    {
    public:
        using FileBase::FileBase;

        bool Open(const std::string& filename) override
        {
            m_stream.clear();
            m_stream.open(filename, std::ios::ate | std::ios::in);
            if (!m_stream.is_open() || m_stream.fail())
            {
                m_isValid = false;
                Logger::ErrorMessage("Could not open file -> %s", filename.c_str());
                return false;
            }

            m_isValid = true;
            m_dataLen = m_stream.tellg();
            m_stream.seekg(0, std::iostream::beg);
            return true;
        }

        std::vector<char> GetBytes() noexcept
        {
            if(m_isValid)
            {
                std::vector<char> data(m_dataLen);
                m_stream.read(data.data(), m_dataLen);
                return data;
            }
            return {};
        }

        std::vector<std::string> GetLines() noexcept
        {
            if (m_isValid)
            {
                std::vector<std::string> lines;
                std::string line;
                while (std::getline(m_stream, line))
                    lines.push_back(line);

                return lines;
            }
            return {};
        }

        // Reads the entire file to a string and returns it.
        std::string GetString() noexcept
        {
            if (m_isValid)
            {
                std::string str(m_dataLen, 0);
                m_stream.read(&str[0], m_dataLen);
                return str;
            }
            else
                return "";
        }

        // Reads the entire file to a string and returns it.
        static std::string GetString(const std::string &filename)
        {
            std::ifstream file(filename, std::ios::ate);

            if (file.is_open())
            {
                const auto size = file.tellg();
                file.seekg(0, std::ios::beg);
                std::string str(size, 0);
                file.read(&str[0], size);
                return str;
            }
            else
                return "";
        }
    };

    class OutFile : public FileBase
    {
    public:
        using FileBase::FileBase;
        bool Open(const std::string& filename) override
        {
            m_stream.clear();
            m_stream.open(filename, std::ios::out);
            if (!m_stream.is_open() || m_stream.fail())
            {
                m_isValid = false;
                Logger::ErrorMessage("Could not open file -> %s", filename.c_str());
                return false;
            }

            m_isValid = true;
            return true;
        }
        void Write(const std::string& string)
        {
            if(m_stream.is_open() && m_stream.good())
            {
                m_stream.write(string.c_str(), string.size());
                m_dataLen += string.size();
            }
        }
        void Write(const char* data, ui64 dataSize)
        {
            if(data && m_stream.is_open() && m_stream.good())
            {
                m_stream.write(data, dataSize);
                m_dataLen += dataSize;
            }
        }
    };
}