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

    Image::Image(i32 w, i32 h, i32 Bpp, const ui8* data) noexcept
    : m_width(w), m_height(h), m_Bpp(Bpp), m_data(new ui8[w*h*Bpp])
    {
        LoadFromMemory(w, h, Bpp, data);
    }

    Image::Image(const ui8* data, ui32 s) noexcept
    : m_width(0), m_height(0), m_Bpp(0), m_data(nullptr)
    {
        LoadFromData(data, s);
    }

    Image::Image(const String& fileName, i32 w, i32 h) noexcept
    : m_width(0), m_height(0), m_Bpp(0), m_data(nullptr)
    {
        LoadFromFile(fileName, w, h);
    }

    Image::Image(const Image& image) noexcept
    : m_width(image.m_width), m_height(image.m_height), m_Bpp(image.m_Bpp), m_data(new ui8[m_width*m_height*m_Bpp])
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
        m_data = new ui8[m_width * m_height * m_Bpp];
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

    bool Image::LoadFromFile(const String& fileName, i32 w, i32 h) noexcept
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

    void Image::LoadFromMemory(i32 w, i32 h, i32 Bpp, const ui8* data) noexcept
    {
        if((m_width * m_height) != (w * h) || m_Bpp != Bpp)
        {
            delete[] m_data;
            m_data = new ui8[w*h*Bpp];
        }

        if(data)
            memcpy(m_data, data, w*h*Bpp);

        if(m_flipOnRead)
            FlipVertically();

        m_width = w;
        m_height = h;
        m_Bpp = Bpp;
    }

    bool Image::LoadFromData(const ui8 *data, ui32 dataSize) noexcept
    {
        delete[] m_data; m_data = nullptr;
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

        if(m_flipOnWrite)
            FlipVertically();

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

        if(m_flipOnWrite)
            FlipVertically();

        return returnValue;
    }

    bool Image::Resize(i32 requestedWidth, i32 requestedHeight) noexcept
    {
        if(m_data == nullptr || m_width == 0 || m_height == 0 ||
        (requestedWidth == (unsigned)m_width && requestedHeight == (unsigned)m_height) || (requestedWidth == 0 && requestedHeight == 0))
            return false;

        if(requestedWidth == 0)
            requestedWidth = requestedHeight * m_height / m_width;

        else if(requestedHeight == 0)
            requestedHeight = requestedWidth * m_width / m_height;

        auto* newData = new ui8[requestedWidth * requestedHeight * m_Bpp];
        stbir_resize_uint8(m_data, m_width, m_height, 0, newData, requestedWidth, requestedHeight, 0, m_Bpp);
        delete[] m_data;
        m_data = newData;
        m_width = requestedWidth;
        m_height = requestedHeight;
        return m_data != nullptr;
    }

    void Image::FlipVertically() noexcept
    {
        if(m_data == nullptr)
            return;

        auto* row = new ui8[m_width * m_Bpp];
        const size_t rowLen = m_width*m_Bpp;
        const size_t height2 = m_height/2;

        for(size_t i = 0; i < height2; ++i)
        {
            std::memcpy(row, m_data+i*rowLen, rowLen);
            std::memcpy(m_data+i*rowLen, m_data+(m_height-1-i)*rowLen, rowLen);
            std::memcpy(m_data+(m_height-1-i)*rowLen, row, rowLen);
        }

        delete[] row;
    }

    void Image::SetFlipOnLoad(bool flip)
    {
        m_flipOnRead = flip;
    }

    void Image::SetFlipOnWrite(bool flip)
    {
        m_flipOnWrite = flip;
    }

    bool Image::LoadSvg(const String& filename, i32 requestedWidth, i32 requestedHeight)
    {
        auto data = InFile::GetString(filename);
        return LoadSvg(reinterpret_cast<const ui8 *>(data.data()), static_cast<ui32>(data.size()), requestedWidth, requestedHeight);
    }

    bool Image::LoadSvg(const ui8* data, ui32 dataSize, i32 requestedWidth, i32 requestedHeight)
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
        ssize_t pos = 0;

        for(pos = static_cast<ssize_t>(filename.length()) - 1; pos >= 0; --pos)
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