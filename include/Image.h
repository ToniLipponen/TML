#pragma once
#include "Types.h"

namespace tml
{
    // @brief Supported file types: Jpeg / jpg, png, bmp, ppm, tiff, targa.
    class Image
    {
    public:
        Image();
        Image(i32 w, i32 h, ui8* data = nullptr);
        Image(cstring filename);

        constexpr i32 GetWidth()             const noexcept { return m_width;  }
        constexpr i32 GetHeight()            const noexcept { return m_height; }
        constexpr i32 GetBpp()               const noexcept { return m_Bpp;    }
        constexpr ui8 const* const GetData() const noexcept { return m_data; }

        bool FromFile(cstring filename);
        bool FromMemory(i32 w, i32 h, ui8* data); // @brief Creates image from pixel data in memory
        bool FromData(ui32 data_size, ui8* data); // @brief Creates image from raw file data in memory

        bool ToFile(cstring filename); // @brief Saves image to file. Uses postfix to deduce image file type. eg. .jpg .png ..
    private:
        i32 m_width, m_height, m_Bpp;
        ui8* m_data;
    };
}