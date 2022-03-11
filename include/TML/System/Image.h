#pragma once
#include <TML/Types.h>
#include <TML/System/String.h>

namespace tml
{
    class Image
    {
    public:
        Image() noexcept;
        Image(i32 w, i32 h, i32 Bpp, const ui8* data) noexcept;
        Image(const ui8* data, ui32 dataSize) noexcept;
        Image(const String& fileName, ui32 requestedWidth = 0, ui32 requestedHeight = 0) noexcept;
        Image(const Image& image) noexcept;
        Image(Image&& image) noexcept;
        ~Image() noexcept;

        Image& operator=(const Image& rhs) noexcept;
        Image& operator=(Image&& rhs) noexcept;

        inline constexpr i32  GetWidth()  const noexcept { return m_width;  } //!< @returns Width of the image.
        inline constexpr i32  GetHeight() const noexcept { return m_height; } //!< @returns Height of the image.
        inline constexpr i32  GetBpp()    const noexcept { return m_Bpp;    } //!< @returns Bytes per pixel.
        inline constexpr ui8* GetData()   const noexcept { return m_data;   } //!< @returns Pointer to pixel data.

        /** @brief Loads image from file.
         *  @returns true if successful, or false if unsuccessful. */
        bool LoadFromFile(const String& fileName, ui32 requestedWidth = 0, ui32 requestedHeight = 0) noexcept;

        /** @brief Creates image from pixel data in memory.
         *  @param w Width in pixels.
         *  @param h Height in pixels.
         *  @param Bpp Bytes per pixel.
         *  @param data Pointer to pixel data. */
        void LoadFromMemory(i32 w, i32 h, i32 Bpp, const ui8* data) noexcept;

        /** @brief Creates image from raw file data in memory.
         *  @returns true if successful, or false if unsuccessful. */
        bool LoadFromData(const ui8* data, ui32 dataSize) noexcept;

        /** @brief Saves image to file. Uses postfix to deduce image file type. eg. .jpg .png .webp ...
         *  @returns true if successful, or false if unsuccessful. */
        bool WriteToFile(const String& fileName, i32 quality = 90) const noexcept;

        /** @brief Resize the image to requested size.
         *  @returns true if successful, or false if unsuccessful. */
        bool Resize(ui32 requestedWidth = 0, ui32 requestedHeight = 0) noexcept;
    private:
        enum ImageType { None, Jpg, Png, Bmp, Tga, Pic, Pnm, Webp, Svg };
        bool LoadSvg(const String& filename, ui32 requestedWidth = 0, ui32 requestedHeight = 0);
        bool LoadSvg(const ui8* data, ui32 dataSize, ui32 requestedWidth = 0, ui32 requestedHeight = 0);
        bool LoadWebp(const String& filename) noexcept;
        bool LoadWebp(const ui8* data, ui32 size) noexcept;
        bool SaveWebp(const String& filename, i32 quality = 90) const noexcept;
        static ImageType GetTypeFromFilename(const String& filename) noexcept;
        i32 m_width = 0, m_height = 0, m_Bpp = 0;
        ui8* m_data = nullptr;
    };
}