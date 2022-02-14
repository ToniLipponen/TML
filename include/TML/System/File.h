#pragma once
#include <TML/System/Platform.h>
#include <TML/System/String.h>
#include <TML/System/Logger.h>

#include <vector>
#include <fstream>

namespace tml
{
    class FileBase
    {
    public:
        FileBase() = default;
        explicit FileBase(const String& filename)
        {
            m_stream.open(filename.c_str(), std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
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

        virtual bool Open(const String& filename) = 0;

        static bool FileExists(const String& filename)
        {
            std::ifstream file(filename.c_str());
            const bool exists = file.is_open();
            file.close();
            return exists;
        }

        ui32 FileSize() const noexcept
        {
            return m_dataLen;
        }

        static i64 FileSize(const String &filename)
        {
            std::ifstream file(filename.c_str());
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

        bool Open(const String& filename) override
        {
            m_stream.clear();
            m_stream.open(filename.c_str(), std::ios::ate | std::ios::in);

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

        void GetBytes(std::vector<char>& dest) noexcept
        {
            if(m_isValid)
            {
                dest.reserve(m_dataLen);
                m_stream.read(dest.data(), m_dataLen);
            }
        }

        void Read(void* dest, ui64 bytes) noexcept
        {
            m_stream.read((char*)dest, bytes);
        }

        /// @brief Reads every line of the file into a vector.
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

        /// @brief Reads the entire file to a string and returns it.
        String GetString() noexcept
        {
            if (m_isValid)
            {
                String str;
                m_stream.read(str.data(), m_dataLen);
                return str;
            }
            else
                return "";
        }

        /// @brief Opens a file and reads its contents to a string and returns it.
        static std::string GetString(const String &filename)
        {
            std::ifstream file(filename.c_str(), std::ios::ate);

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

        /// @brief Opens an output file.
        /// @returns true if successful, otherwise returns false.
        bool Open(const String& filename) override
        {
            m_stream.clear();
            m_stream.open(filename.c_str(), std::ios::out);
            if (!m_stream.is_open() || m_stream.fail())
            {
                m_isValid = false;
                Logger::ErrorMessage("Could not open file -> %s", filename.c_str());
                return false;
            }

            m_isValid = true;
            return true;
        }

        /// @brief Appends string to the file.
        void Write(const String& string)
        {
            if(m_stream.is_open() && m_stream.good())
            {
                m_stream.write(string.c_str(), string.size());
                m_dataLen += string.size();
            }
        }

        /// @brief Appends n amount of byte to the file.
        /// @param data pointer to data you want to append.
        /// @param n the size of data in bytes.
        void Write(const char* data, ui64 n)
        {
            if(data && m_stream.is_open() && m_stream.good())
            {
                m_stream.write(data, n);
                m_dataLen += n;
            }
        }
    };
}