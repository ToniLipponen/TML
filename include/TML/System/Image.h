#pragma once
#include <cstdint>
#include <TML/System/String.h>
#include <TML/Export.h>

namespace tml
{
    class TML_API Image
    {
    public:
        Image() noexcept;
        Image(int32_t w, int32_t h, int32_t Bpp, const uint8_t* data) noexcept;
        Image(const uint8_t* data, uint32_t dataSize) noexcept;
        Image(const String& fileName, int32_t requestedWidth = 0, int32_t requestedHeight = 0) noexcept;
        Image(const Image& image) noexcept;
        Image(Image&& image) noexcept;
        ~Image() noexcept;

        Image& operator=(const Image& rhs) noexcept;
        Image& operator=(Image&& rhs) noexcept;

        inline constexpr int32_t  GetWidth()  const noexcept { return m_width;  } //!< @returns Width of the image.
        inline constexpr int32_t  GetHeight() const noexcept { return m_height; } //!< @returns Height of the image.
        inline constexpr int32_t  GetBpp()    const noexcept { return m_Bpp;    } //!< @returns Bytes per pixel.
        inline constexpr uint8_t* GetData()   const noexcept { return m_data;   } //!< @returns Pointer to pixel data.

        /** @brief Loads image from file.
         *  @returns true if successful, or false if unsuccessful. */
        bool LoadFromFile(const String& fileName, int32_t requestedWidth = 0, int32_t requestedHeight = 0) noexcept;

        /** @brief Creates image from pixel data in memory.
         *  @param w Width in pixels.
         *  @param h Height in pixels.
         *  @param Bpp Bytes per pixel.
         *  @param data Pointer to pixel data. */
        bool LoadFromMemory(int32_t w, int32_t h, int32_t Bpp, const uint8_t* data) noexcept;

        /** @brief Creates image from raw file data in memory.
         *  @returns true if successful, or false if unsuccessful. */
        bool LoadFromData(const uint8_t* data, uint32_t dataSize) noexcept;

        /** @brief Saves image to file. Uses postfix to deduce image file type. eg. .jpg .png .webp ...
         *  @returns true if successful, or false if unsuccessful. */
        bool WriteToFile(const String& fileName, int32_t quality = 90) noexcept;

        /** @brief Resize the image to requested size.
         *  @returns true if successful, or false if unsuccessful. */
        bool Resize(int32_t requestedWidth = 0, int32_t requestedHeight = 0) noexcept;
        bool FlipVertically() noexcept;
        void SetFlipOnLoad(bool flip) noexcept;
        void SetFlipOnWrite(bool flip) noexcept;

    private:
        enum ImageType { None, Jpg, Png, Bmp, Tga, Pic, Pnm, Svg };
        bool LoadSvg(const String& filename, int32_t requestedWidth = 0, int32_t requestedHeight = 0) noexcept;
        bool LoadSvg(const uint8_t* data, uint32_t dataSize, int32_t requestedWidth = 0, int32_t requestedHeight = 0) noexcept;
        static ImageType GetTypeFromFilename(const String& filename) noexcept;

    private:
        int32_t m_width = 0, m_height = 0, m_Bpp = 0;
        uint8_t* m_data = nullptr;
        bool m_flipOnRead = false;
        bool m_flipOnWrite = false;
    };
}