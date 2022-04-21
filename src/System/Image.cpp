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

namespace tml
{
    Image::Image() noexcept
    : m_width(0), m_height(0), m_Bpp(0), m_data(nullptr)
    {

    }

    Image::Image(int32_t w, int32_t h, int32_t Bpp, const uint8_t* data) noexcept
    : m_width(w), m_height(h), m_Bpp(Bpp), m_data(new uint8_t[w*h*Bpp])
    {
        LoadFromMemory(w, h, Bpp, data);
    }

    Image::Image(const uint8_t* data, uint32_t s) noexcept
    : m_width(0), m_height(0), m_Bpp(0), m_data(nullptr)
    {
        LoadFromData(data, s);
    }

    Image::Image(const String& fileName, int32_t w, int32_t h) noexcept
    : m_width(0), m_height(0), m_Bpp(0), m_data(nullptr)
    {
        LoadFromFile(fileName, w, h);
    }

    Image::Image(const Image& image) noexcept
    : m_width(image.m_width), m_height(image.m_height), m_Bpp(image.m_Bpp), m_data(new uint8_t[m_width*m_height*m_Bpp])
    {
        if(image.m_data)
            std::memcpy(m_data, image.m_data, m_width*m_height*m_Bpp);
    }

    Image::Image(Image&& image) noexcept
    : m_width(0), m_height(0), m_Bpp(0), m_data(nullptr)
    {
        *this = image;
    }

    Image::~Image() noexcept
    {
        delete[] m_data;
    }

    Image& Image::operator=(const Image& rhs) noexcept
    {
        if(&rhs == this)
            return *this;

        this->m_width = rhs.m_width;
        this->m_height = rhs.m_height;
        this->m_Bpp = rhs.m_Bpp;

        delete[] m_data;
        m_data = new uint8_t[m_width * m_height * m_Bpp];
        if(rhs.m_data)
            std::memcpy(m_data, rhs.m_data, m_width * m_height * m_Bpp);
        return *this;
    }

    Image& Image::operator=(Image&& rhs) noexcept
    {
        delete[] m_data;
        this->m_width = rhs.m_width;
        this->m_height = rhs.m_height;
        this->m_Bpp = rhs.m_Bpp;
        this->m_data = rhs.m_data;

        rhs.m_data = nullptr;
        rhs.m_width = 0;
        rhs.m_height = 0;
        rhs.m_Bpp = 0;
        return *this;
    }

    bool Image::LoadFromFile(const String& fileName, int32_t w, int32_t h) noexcept
    {
        delete[] m_data;
        m_data = nullptr;

        const auto imageType = GetTypeFromFilename(fileName);
        bool returnValue;

        if(imageType == Image::Svg)
        {
            returnValue = LoadSvg(fileName, w, h);
        }
        else
        {
            m_data = stbi_load(fileName.c_str(), &m_width, &m_height, &m_Bpp, 0);
            Resize(w,h);
            returnValue = (m_data != nullptr);
        }

        if(m_flipOnRead)
            FlipVertically();

        return returnValue;
    }

    void Image::LoadFromMemory(int32_t w, int32_t h, int32_t Bpp, const uint8_t* data) noexcept
    {
        if((m_width * m_height) != (w * h) || m_Bpp != Bpp)
        {
            delete[] m_data;
            m_data = new uint8_t[w*h*Bpp];
        }

        if(data)
            memcpy(m_data, data, w*h*Bpp);

        if(m_flipOnRead)
            FlipVertically();

        m_width = w;
        m_height = h;
        m_Bpp = Bpp;
    }

    bool Image::LoadFromData(const uint8_t *data, uint32_t dataSize) noexcept
    {
        delete[] m_data;

        m_data = stbi_load_from_memory(data, static_cast<int>(dataSize), &m_width, &m_height, &m_Bpp, 0);
        bool returnValue = m_data != nullptr;

        if(!returnValue)
            returnValue = LoadSvg(data, dataSize);

        if(m_flipOnRead)
            FlipVertically();

        return returnValue;
    }

    bool Image::WriteToFile(const String& fileName, int quality) noexcept
    {
        const auto type = GetTypeFromFilename(fileName);

        stbi_flip_vertically_on_write(m_flipOnWrite);

        bool returnValue;
        switch(type)
        {
            case Image::Png:
                returnValue = stbi_write_png(fileName.c_str(), m_width, m_height, m_Bpp, m_data, 0) != 0;
                break;

            case Image::Bmp:
                returnValue = stbi_write_bmp(fileName.c_str(), m_width, m_height, m_Bpp, m_data) != 0;
                break;

            case Image::Tga:
                returnValue = stbi_write_tga(fileName.c_str(), m_width, m_height, m_Bpp, m_data) != 0;
                break;

            default:
                returnValue = stbi_write_jpg(fileName.c_str(), m_width, m_height, m_Bpp, m_data, quality) != 0;
        }

        stbi_flip_vertically_on_write(0);

        return returnValue;
    }

    bool Image::Resize(int32_t requestedWidth, int32_t requestedHeight) noexcept
    {
        if(m_data == nullptr || m_width == 0 || m_height == 0 ||
        (requestedWidth == (unsigned)m_width && requestedHeight == (unsigned)m_height) || (requestedWidth == 0 && requestedHeight == 0))
            return false;

        if(requestedWidth == 0)
            requestedWidth = requestedHeight * m_height / m_width;

        else if(requestedHeight == 0)
            requestedHeight = requestedWidth * m_width / m_height;

        auto* newData = new uint8_t[requestedWidth * requestedHeight * m_Bpp];
        stbir_resize_uint8(m_data, m_width, m_height, 0, newData, requestedWidth, requestedHeight, 0, m_Bpp);
        delete[] m_data;
        m_data = newData;
        m_width = requestedWidth;
        m_height = requestedHeight;
        return true;
    }

    bool Image::FlipVertically() noexcept
    {
        if(m_data == nullptr)
            return false;

        const size_t rowLen = m_width * m_Bpp;
        const size_t height2 = m_height / 2;
        auto* row = new uint8_t[rowLen];

        for(size_t i = 0; i < height2; ++i)
        {
            auto* bottom = m_data + (m_height - 1 - i) * rowLen;
            auto* top = m_data + i * rowLen;

            std::memcpy(row, top, rowLen);
            std::memcpy(top, bottom, rowLen);
            std::memcpy(bottom, row, rowLen);
        }

        delete[] row;
        return true;
    }

    void Image::SetFlipOnLoad(bool flip)
    {
        m_flipOnRead = flip;
    }

    void Image::SetFlipOnWrite(bool flip)
    {
        m_flipOnWrite = flip;
    }

    bool Image::LoadSvg(const String& filename, int32_t requestedWidth, int32_t requestedHeight)
    {
        auto data = InFile::GetString(filename);
        return LoadSvg(reinterpret_cast<const uint8_t *>(data.data()), static_cast<uint32_t>(data.size()), requestedWidth, requestedHeight);
    }

    bool Image::LoadSvg(const uint8_t* data, uint32_t dataSize, int32_t requestedWidth, int32_t requestedHeight)
    {
        auto document = lunasvg::Document::loadFromData(reinterpret_cast<const char*>(data), dataSize);

        if(!document)
            return false;

        m_Bpp = 4;
        m_width = requestedWidth;
        m_height = requestedHeight;
        m_data = document->render(m_width, m_height);

        return m_data != nullptr;
    }

    Image::ImageType Image::GetTypeFromFilename(const String &filename) noexcept
    {
        int64_t pos;

        for(pos = static_cast<int64_t>(filename.length()) - 1; pos >= 0; --pos)
        {
            if(filename.at(pos) == '.')
                break;
        }

        if(pos == 0 || pos == -1)
            return Image::None;

        const auto len = filename.length() - pos;
        const auto str = filename.substr(pos, len);

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
}