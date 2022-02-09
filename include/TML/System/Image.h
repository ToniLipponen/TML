#pragma once
#include <TML/Types.h>
#include <string>

namespace tml
{
    class Image
    {
    public:
        Image() noexcept;
        Image(i32 w, i32 h, i32 Bpp, const ui8* data) noexcept;
        Image(const ui8* data, ui32 dataSize) noexcept;
        explicit Image(const std::string& fileName) noexcept;
        Image(const Image& image) noexcept;
        Image(Image&& image) noexcept;
        ~Image() noexcept;

        Image& operator=(const Image& rhs) noexcept;
        Image& operator=(Image&& rhs) noexcept;

        inline constexpr i32  GetWidth()  const noexcept { return m_width;  }
        inline constexpr i32  GetHeight() const noexcept { return m_height; }
        inline constexpr i32  GetBpp()    const noexcept { return m_Bpp;    }
        inline constexpr ui8* GetData()   const noexcept { return m_data;   }

        bool LoadFromFile(const std::string& fileName) noexcept;
        void LoadFromMemory(i32 w, i32 h, i32 Bpp, const ui8* data) noexcept;     /// @brief Creates image from pixel data in memory
        bool LoadFromData(const ui8* data, ui32 dataSize) noexcept;               /// @brief Creates image from raw file data in memory
        bool WriteToFile(const std::string& fileName, i32 quality = 90) noexcept; /// @brief Saves image to file. Uses postfix to deduce image file type. eg. .jpg .png ..
    private:
        enum ImageType { None, Jpg, Png, Bmp, Tga, Pic, Pnm, Webp };
        bool LoadWebp(const std::string& filename) noexcept;
        bool LoadWebp(const ui8* data, ui32 size) noexcept;
        bool SaveWebp(const std::string& filename, i32 quality = 90) noexcept;
        static ImageType GetTypeFromFilename(const std::string& filename) noexcept;
        i32 m_width, m_height, m_Bpp;
        ui8* m_data;
    };
}