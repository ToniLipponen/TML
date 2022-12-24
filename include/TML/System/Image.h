#pragma once
#include <cstdint>
#include <TML/System/String.h>
#include <TML/Export.h>
#include <TML/Graphics/Color.h>

namespace tml
{
    class TML_API Image
    {
    public:
        Image() noexcept;
        Image(int32_t w, int32_t h, const Color* data) noexcept;
        Image(uint32_t w, uint32_t h, Color color = Color::Transparent) noexcept;
        Image(const uint8_t* data, int32_t dataSize) noexcept;
        explicit Image(const String& fileName, int32_t requestedWidth = 0, int32_t requestedHeight = 0) noexcept;

        [[nodiscard]] int32_t GetWidth() const noexcept;
        [[nodiscard]] int32_t GetHeight() const noexcept;
        [[nodiscard]] Vector2ui GetSize() const noexcept;
        [[nodiscard]] Image GetArea(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const noexcept;
        [[nodiscard]] Image GetArea(Vector2ui position, Vector2ui size) const noexcept;
        [[nodiscard]] Color GetPixel(uint32_t x, uint32_t y) const noexcept;
        [[nodiscard]] Color GetPixel(Vector2ui position) const noexcept;
        [[nodiscard]] const Color* GetData() const noexcept;
        [[nodiscard]] Color* GetData() noexcept;

        bool LoadFromFile(const String& fileName, int32_t requestedWidth = 0, int32_t requestedHeight = 0) noexcept;
        bool LoadFromMemory(int32_t w, int32_t h, const Color* data) noexcept;
        bool LoadFromData(const uint8_t* data, int32_t dataSize) noexcept;
        bool WriteToFile(const String& fileName, int32_t quality = 90) noexcept;
        bool Resize(uint32_t requestedWidth = 0, uint32_t requestedHeight = 0) noexcept;
        bool FlipVertically() noexcept;
        void SetFlipOnLoad(bool flip) noexcept;
        void SetFlipOnWrite(bool flip) noexcept;

        void Clear(Color clearColor = Color::Transparent) noexcept;
        void SetPixel(uint32_t x, uint32_t y, Color pixelColor) noexcept;
        void SetPixel(Vector2ui position, Color pixelColor) noexcept;
        void InsertImage(uint32_t x, uint32_t y, const Image& image) noexcept;
        void InsertImage(Vector2ui position, const Image& image) noexcept;

    private:
        enum ImageType { None, Jpg, Png, Bmp, Tga, Pic, Pnm, Svg };
        bool LoadSvg(const String& filename, uint32_t requestedWidth = 0, uint32_t requestedHeight = 0) noexcept;
        bool LoadSvg(const uint8_t* data, uint32_t dataSize, uint32_t requestedWidth = 0, uint32_t requestedHeight = 0) noexcept;
        static ImageType GetTypeFromFilename(const String& filename) noexcept;

    private:
        int32_t m_width = 0, m_height = 0;
        std::vector<Color> m_data;
        bool m_flipOnRead = false;
        bool m_flipOnWrite = false;
    };
}

/**
 * @class tml::Image
 *
 * Utility class for loading and manipulating image data.
 *
 * @fn bool tml::Image::LoadFromFile()
 * @brief Attempt to load an image from a file on the hard drive.
 *
 * If you wish to scale the image, or if the image type is svg, you can provide the function with
 * \b requestedWidth and \b requestedHeight and the image will be scaled to those given dimensions.
 *
 * @param fileName
 * The path to an image file.
 *
 * @param requestedWidth
 * Optional argument if you wish to scale the image.
 * @param requestedHeight
 * Optional argument if you wish to scale the image.
 *
 * You can also provide only the width and the image will be scaled to that width while retaining the aspect ratio.
 * @returns False if the file is not a valid image file, or if the file type is unsupported.
 * @returns True if successful.
 */