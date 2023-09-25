#define STBI_WINDOWS_UTF8
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <stb/stb_image_resize.h>

#include <lunasvg.h>
#include <TML/System/Image.h>
#include <TML/System/File.h>
#include <cstring>
#include <algorithm>

namespace tml
{
    Image::Image() noexcept
    : m_width(0), m_height(0)
    {

    }

    Image::Image(int32_t w, int32_t h, const Color* data) noexcept
    : m_width(w), m_height(h)
    {
        LoadFromMemory(w, h, data);
    }

    Image::Image(uint32_t w, uint32_t h, Color color) noexcept
    : m_width(static_cast<int32_t>(w)), m_height(static_cast<int32_t>(h)), m_data(w*h, color)
    {

    }

    Image::Image(const uint8_t* data, int32_t s) noexcept
    : m_width(0), m_height(0)
    {
        LoadFromData(data, s);
    }

    Image::Image(const String& fileName, int32_t w, int32_t h) noexcept
    : m_width(0), m_height(0)
    {
        LoadFromFile(fileName, w, h);
    }

    [[nodiscard]] int32_t Image::GetWidth() const noexcept
    {
        return m_width;
    }
    
    [[nodiscard]] int32_t Image::GetHeight() const noexcept
    {
        return m_height;
    }

    [[nodiscard]] Vector2ui Image::GetSize() const noexcept
    {
        return {m_width, m_height};
    }

    [[nodiscard]] Image Image::GetArea(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const noexcept
    {
        Image image(width, height);

        for(int32_t i = 0; i < height; i++)
        {
            for(int32_t j = 0; j < width; j++)
            {
                image.SetPixel(j, i, GetPixel(x+j, y+i));
            }
        }

        return image;
    }

    Image Image::GetArea(Vector2ui position, Vector2ui size) const noexcept
    {
        return GetArea(position.x, position.y, size.x, size.y);
    }

    Color Image::GetPixel(uint32_t x, uint32_t y) const noexcept
    {
        if(x < m_width && y < m_height)
        {
            return m_data[y * m_width + x];
        }

        return {};
    }

    Color Image::GetPixel(Vector2ui position) const noexcept
    {
        return GetPixel(position.x, position.y);
    }
    
    [[nodiscard]] const Color* Image::GetData() const noexcept
    {
        return m_data.data();
    }

    [[nodiscard]] Color* Image::GetData() noexcept
    {
        return m_data.data();
    }

    bool Image::LoadFromFile(const String& fileName, int32_t w, int32_t h) noexcept
    {
        if(auto pixels = reinterpret_cast<Color*>(stbi_load(fileName.c_str(), &m_width, &m_height, nullptr, 4)))
        {
            m_data = std::vector<Color>(pixels, pixels + m_width * m_height);
            Resize(w,h);
            stbi_image_free(pixels);
        }
        else if(LoadSvg(fileName, w, h))
        {

        }
        else
        {
            return false;
        }

        if(m_flipOnRead)
        {
            FlipVertically();
        }

        return true;
    }

    bool Image::LoadFromMemory(int32_t w, int32_t h, const Color* data) noexcept
    {
		if(w > 0 && h > 0)
		{
			m_width = w;
			m_height = h;

			m_data.resize(w * h);

			if(data)
			{
				memcpy(&m_data[0], data, static_cast<uint64_t>(w) * h * sizeof(Color));
			}

			if(m_flipOnRead)
			{
				FlipVertically();
			}

			return true;
		}

        return false;
    }

    bool Image::LoadFromData(const uint8_t *data, int32_t dataSize) noexcept
    {
        if(data)
        {
            return false;
        }

        if(auto pixels = reinterpret_cast<Color*>(stbi_load_from_memory(data, dataSize, &m_width, &m_height, nullptr, 4)))
        {
            m_data = std::vector<Color>(pixels, pixels + m_width * m_height * 4);
            stbi_image_free(pixels);
        }
        else
        {
            if(!LoadSvg(data, dataSize))
            {
                return false;
            }
        }

        if(m_flipOnRead)
        {
            FlipVertically();
        }

        return true;
    }

    bool Image::WriteToFile(const String& fileName, int quality) noexcept
    {
        const auto type = GetTypeFromFilename(fileName);

        stbi_flip_vertically_on_write(m_flipOnWrite);

        bool returnValue;

        switch(type)
        {
            case Image::Png:
                returnValue = stbi_write_png(fileName.c_str(),
                                             m_width,
                                             m_height,
                                             4,
                                             m_data.data(),
                                             m_width * 4) != 0;
                break;

            case Image::Bmp:
                returnValue = stbi_write_bmp(fileName.c_str(),
                                             m_width,
                                             m_height,
                                             4,
                                             m_data.data()) != 0;
                break;

            case Image::Tga:
                returnValue = stbi_write_tga(fileName.c_str(),
                                             m_width,
                                             m_height,
                                             4,
                                             m_data.data()) != 0;
                break;

            default:
                returnValue = stbi_write_jpg(fileName.c_str(),
                                             m_width,
                                             m_height,
                                             4,
                                             m_data.data(),
                                             quality) != 0;
                break;
        }

        stbi_flip_vertically_on_write(0);

        return returnValue;
    }

    bool Image::Resize(uint32_t requestedWidth, uint32_t requestedHeight) noexcept
    {
        auto newWidth = static_cast<int32_t>(requestedWidth);
		auto newHeight = static_cast<int32_t>(requestedHeight);

        if(m_data.empty() || m_width == 0 || m_height == 0 || (requestedWidth == m_width && requestedHeight == m_height)
        || (requestedWidth == 0 && requestedHeight == 0))
        {
            return false;
        }

        if(requestedWidth <= 0)
        {
            newWidth = static_cast<int32_t>(requestedHeight * m_height / m_width);
        }
        else if(requestedHeight <= 0)
        {
            newHeight = static_cast<int32_t>(requestedWidth * m_width / m_height);
        }

        std::vector<Color> newData;
        newData.resize(static_cast<uint64_t>(newWidth) * newHeight);

        stbir_resize_uint8(reinterpret_cast<unsigned char*>(m_data.data()),
                           m_width,
                           m_height,
                           m_width * sizeof(Color),
                           reinterpret_cast<unsigned char*>(newData.data()),
                           newWidth,
                           newHeight,
                           newWidth * sizeof(Color),
                           4);

        m_data = std::move(newData);
        m_width = newWidth;
        m_height = newWidth;
 
        return true;
    }

    bool Image::FlipVertically() noexcept
    {
        if(m_data.empty() || m_width == 0 || m_height == 0)
        {
            return false;
        }

        stbi__vertical_flip(m_data.data(), m_width, m_height, sizeof(Color));

        return true;
    }

    void Image::SetFlipOnLoad(bool flip) noexcept
    {
        m_flipOnRead = flip;
    }

    void Image::SetFlipOnWrite(bool flip) noexcept
    {
        m_flipOnWrite = flip;
    }

    bool Image::LoadSvg(const String& filename, uint32_t requestedWidth, uint32_t requestedHeight) noexcept
    {
        if(auto data = File::ReadBytes(filename.cpp_str()); !data.empty())
        {
            auto* bytes = reinterpret_cast<const uint8_t *>(data.data());
            auto bytesLength = static_cast<uint32_t>(data.size());

            return LoadSvg(bytes, bytesLength, requestedWidth, requestedHeight);
        }

        return false;
    }

    bool Image::LoadSvg(const uint8_t* data, uint32_t dataSize, uint32_t requestedWidth, uint32_t requestedHeight) noexcept
    {
        if(auto document = lunasvg::Document::loadFromData(reinterpret_cast<const char*>(data), dataSize))
        {
            auto bitmap = document->renderToBitmap(requestedWidth, requestedHeight);

            if(bitmap.valid())
            {
                bitmap.convertToRGBA();
                m_width = static_cast<int32_t>(bitmap.width());
                m_height = static_cast<int32_t>(bitmap.height());
                auto bitmapSize = m_width * m_height;
                m_data = std::vector<Color>(bitmapSize);
                memcpy(m_data.data(), bitmap.data(), bitmapSize * sizeof(Color));
            
                return true;
            }
        }

        return false;
    }

    Image::ImageType Image::GetTypeFromFilename(const String &filename) noexcept
    {
        if(filename.empty())
        {
            return Image::None;
        }

        int64_t pos = static_cast<int64_t>(filename.length()) - 1;

        for(; pos >= 0; --pos)
        {
            if(filename.at(pos) == '.')
            {
                break;
            }
        }

        if(pos == 0 || pos == -1)
        {
            return Image::None;
        }

        const auto len = filename.length() - pos;
        auto str = filename.substr(pos, len).cpp_str();
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);

        if(str == ".png")
            return Image::Png;
        else if(str == ".jpg" || str == ".jpeg")
            return Image::Jpg;
        else if(str == ".bmp")
            return Image::Bmp;
        else if(str == ".tga")
            return Image::Tga;
        else if(str == ".pic")
            return Image::Pic;
        else if(str == ".ppm")
            return Image::Pnm;
        else if(str == ".svg")
            return Image::Svg;

        return Image::None;
    }

    void Image::Clear(Color clearColor) noexcept
    {
        std::for_each(m_data.begin(), m_data.end(), [&clearColor](Color& pixel){ pixel = clearColor; });
    }

    void Image::SetPixel(uint32_t x, uint32_t y, Color pixelColor) noexcept
    {
        if(x < m_width && y < m_height)
        {
            m_data[y * m_width + x] = pixelColor;
        }
    }

    void Image::SetPixel(Vector2ui position, Color pixelColor) noexcept
    {
        SetPixel(position.x, position.y, pixelColor);
    }

    void Image::InsertImage(uint32_t x, uint32_t y, const Image& image) noexcept
    {
        for(int32_t i = 0; i < image.m_width; i++)
        {
            for(int32_t j = 0; j < image.m_height; j++)
            {
                if(j + x < m_width && i + y < m_height)
                {
                    m_data[(i + y) * m_width + j + x] = image.m_data[i * image.m_width + j];
                }
            }
        }
    }

    void Image::InsertImage(Vector2ui position, const Image& image) noexcept
    {
        InsertImage(position.x, position.y, image);
    }
}