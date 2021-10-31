#pragma once
#include "Types.h"
#include <string>

namespace tml
{
    class Image
    {
    public:
        Image();
        Image(i32 w, i32 h, i32 Bpp, const ui8* data);
        Image(const ui8* data, ui32 data_size);
        Image(const std::string& fileName);
        Image(const Image& image);
        Image(Image&& image);
        ~Image();

        Image& operator=(const Image& rhs) noexcept;
        Image& operator=(Image&& rhs) noexcept;

        inline constexpr i32  GetWidth()             const noexcept { return m_width;  }
        inline constexpr i32  GetHeight()            const noexcept { return m_height; }
        inline constexpr i32  GetBpp()               const noexcept { return m_Bpp;    }
        inline constexpr ui8* GetData()              const noexcept { return m_data;   }
        inline constexpr ui8* GetData()                    noexcept { return m_data;   }


        bool LoadFromFile(const std::string& fileName);
        void LoadFromMemory(i32 w, i32 h, i32 Bpp, const ui8* data); // @brief Creates image from pixel data in memory
        bool LoadFromData(const ui8* data, ui32 dataSize); // @brief Creates image from raw file data in memory

        bool SaveToFile(const std::string& fileName); // @brief Saves image to file. Uses postfix to deduce image file type. eg. .jpg .png ..
    private:
        i32 m_width, m_height, m_Bpp;
        ui8* m_data;
    };
}