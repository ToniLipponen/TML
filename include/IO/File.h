#pragma once
#include <list>
#include <string>
#include <vector>
#include <fstream>
#include <exception>
#include <iostream>

using ui32 = unsigned int;

namespace tml {
    class File {
    public:
        using Byte = char;
    public:
        File() {

        }

        virtual ~File() {
            delete[] m_data;
            if (m_in_stream.is_open())
                m_in_stream.close();
        }

        void Open(const std::string &filename) {
            m_in_stream.open(filename);
            if (!m_in_stream.is_open()) {
                m_is_valid = false;
                std::printf("ERROR: Failed to open file -> %s\n", filename.c_str());
                exit(1);
            } else {
                m_is_valid = true;
                m_filename = std::move(filename);
                m_in_stream.seekg(0, std::iostream::end);
                m_data_len = m_in_stream.tellg();
                m_in_stream.seekg(0, std::iostream::beg);
            }
        }

        void Create(const std::string &filename) {
            m_filename = std::move(filename);
        }

        Byte *GetBytes() {
            if (m_is_valid) {
                if (m_data)
                    return m_data;
                m_in_stream.open(m_filename, std::ios::binary);
                if (m_in_stream.is_open()) {
                    m_data = new Byte[m_data_len];
                    m_in_stream.read(m_data, m_data_len);
                    m_in_stream.close();
                }
            } else
                m_data = nullptr;

            return m_data;
        }

        std::vector<std::string> GetLines() {
            if (m_is_valid) {
                std::vector<std::string> lines;
                lines.reserve(50);
                std::string line;
                while (std::getline(m_in_stream, line)) {
                    lines.push_back(line);
                }
                return lines;
            } else {
                std::cerr << "Tried to get lines from an invalid file\n";
                exit(1);
            }
        }

        // Reads the entire file to a string and returns it.
        std::string GetString() {
            if (m_is_valid) {
                std::string lines;
                std::string line;
                m_in_stream.open(m_filename);
                if (!m_in_stream.is_open()) {
                    throw "Error: Could not open file";
                }
                while (std::getline(m_in_stream, line)) {
                    lines += line + "\n";
                }
                return lines;
            } else {
                exit(1);
            }
        }

        // Reads the entire file to a string and returns it.
        static std::string GetString(const std::string &filename) {
            std::ifstream file(filename);
            if (file.is_open()) {
                std::string lines;
                std::string line;
                while (std::getline(file, line)) {
                    lines += line + "\n";
                }
                return lines;
            } else {
                exit(1);
            }
        }

        constexpr ui32 FileSize() const noexcept {
            return m_data_len;
        }

        static ui32 FileSize(const std::string &filename) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                exit(1);
            }

            file.seekg(0, std::iostream::end);
            ui32 size = file.tellg();
            file.close();
            return size;
        }

        static bool Exists(const std::string &filename) {
            std::fstream file(filename);
            bool exists = file.is_open();
            file.close();
            return exists;
        }

    private:
        std::ifstream m_in_stream;
        std::string m_filename;
        Byte *m_data = nullptr;
        ui32 m_data_len = 0;
        bool m_is_valid = false;
    };
};