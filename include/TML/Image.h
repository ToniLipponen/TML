#pragma once
#include "Types.h"
#include <string>

namespace tml
{
    class Image
    {
    public:
        Image();
        Image(i32 w, i32 h, i32 Bpp, ui8* data);
        Image(const std::string& filename);
        Image(const Image& image);
        Image(Image&& image);
        ~Image();

        Image& operator=(const Image& rhs);
        Image& operator=(Image&& rhs);

        inline constexpr i32  GetWidth()             const noexcept { return m_width;  }
        inline constexpr i32  GetHeight()            const noexcept { return m_height; }
        inline constexpr i32  GetBpp()               const noexcept { return m_Bpp;    }
        inline constexpr ui8* GetData()              const noexcept { return m_data; }

        bool LoadFromFile(const std::string& filename);
        void LoadFromMemory(i32 w, i32 h, i32 Bpp, ui8* data); // @brief Creates image from pixel data in memory
        bool LoadFromData(ui32 data_size, ui8* data); // @brief Creates image from raw file data in memory

        bool SaveToFile(const std::string& filename); // @brief Saves image to file. Uses postfix to deduce image file type. eg. .jpg .png ..
    private:
        i32 m_width, m_height, m_Bpp;
        ui8* m_data;
    };
}