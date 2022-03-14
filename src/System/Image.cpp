#define STBI_WINDOWS_UTF8
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <stb/stb_image_resize.h>

#include <libwebp/src/webp/decode.h>
#include <libwebp/src/webp/encode.h>
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
        if(data)
            std::memcpy(m_data, data, w*h*Bpp);
    }

    Image::Image(const ui8* data, ui32 s) noexcept
    : m_width(0), m_height(0), m_Bpp(0), m_data(nullptr)
    {
        LoadFromData(data, s);
    }

    Image::Image(const String& fileName, ui32 w, ui32 h) noexcept
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

    bool Image::LoadFromFile(const String& fileName, ui32 w, ui32 h) noexcept
    {
        delete[] m_data;
        m_data = nullptr;

        const auto imageType = GetTypeFromFilename(fileName);
        switch(imageType)
        {
            case Image::Webp:
                Resize(w,h);
                return LoadWebp(fileName);

            case Image::Svg:
                return LoadSvg(fileName, w, h);

            default:
                m_data = stbi_load(fileName.c_str(), &m_width, &m_height, &m_Bpp, 0);
                Resize(w,h);
                return (m_data != nullptr);
        }
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
        m_width = w;
        m_height = h;
        m_Bpp = Bpp;
    }

    bool Image::LoadFromData(const ui8 *data, ui32 dataSize) noexcept
    {
        delete[] m_data; m_data = nullptr;
        m_data = stbi_load_from_memory(data, static_cast<int>(dataSize), &m_width, &m_height, &m_Bpp, 0);
        if(m_data == nullptr && !LoadWebp(data, dataSize))
            return LoadSvg(data, dataSize);
        else
            return true;
    }

    bool Image::WriteToFile(const String& fileName, int quality) const noexcept
    {
        const auto type = GetTypeFromFilename(fileName);

        switch(type)
        {
            case Image::Png:
                return stbi_write_png(fileName.c_str(), m_width, m_height, m_Bpp, m_data, 0) != 0;

            case Image::Bmp:
                return stbi_write_bmp(fileName.c_str(), m_width, m_height, m_Bpp, m_data) != 0;

            case Image::Tga:
                return stbi_write_tga(fileName.c_str(), m_width, m_height, m_Bpp, m_data) != 0;

            case Image::Webp:
                return SaveWebp(fileName, quality);

            default:
                return stbi_write_jpg(fileName.c_str(), m_width, m_height, m_Bpp, m_data, quality) != 0;
        }
    }

    bool Image::Resize(ui32 requestedWidth, ui32 requestedHeight) noexcept
    {
        if(m_data == nullptr || m_width == 0 || m_height == 0 || (requestedWidth == m_width && requestedHeight == m_height) || (requestedWidth == 0 && requestedHeight == 0))
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

    bool Image::LoadSvg(const String& filename, ui32 requestedWidth, ui32 requestedHeight)
    {
        auto data = InFile::GetString(filename);
        return LoadSvg(reinterpret_cast<const ui8 *>(data.data()), static_cast<ui32>(data.size()), requestedWidth, requestedHeight);
    }

    bool Image::LoadSvg(const ui8* data, ui32 dataSize, ui32 requestedWidth, ui32 requestedHeight)
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

    bool Image::LoadWebp(const String &filename) noexcept
    {
        InFile file;
        file.Open(filename);
        std::vector<char> data;
        file.GetBytes(data);

       return LoadWebp(reinterpret_cast<const ui8*>(data.data()), data.size());
    }

    bool Image::LoadWebp(const ui8 *data, ui32 size) noexcept
    {
        WebPBitstreamFeatures features{};
        auto status = WebPGetFeatures(data, size, &features);

        if(status == VP8_STATUS_OK)
        {
            m_width = features.width;
            m_height = features.height;
            if(features.has_alpha)
            {
                m_Bpp = 4;
                m_data = WebPDecodeRGBA(data, size, &m_width, &m_height);
            }
            else
            {
                m_Bpp = 3;
                m_data = WebPDecodeRGB(data, size, &m_width, &m_height);
            }
            return true;
        }
        return false;
    }

    bool Image::SaveWebp(const String& filename, int quality) const noexcept
    {
        ui8* output = nullptr;
        ui64 size = 0;

        if(m_Bpp == 3)
            size = WebPEncodeRGB(m_data, m_width, m_height, m_width * m_Bpp, static_cast<float>(quality), &output);
        else if(m_Bpp == 4)
            size = WebPEncodeRGBA(m_data, m_width, m_height, m_width * m_Bpp, static_cast<float>(quality), &output);

        if(size && output)
        {
            OutFile file;
            file.Open(filename);
            file.Write(reinterpret_cast<const char *>(output), size);
            return true;
        }
        return false;
    }

    Image::ImageType Image::GetTypeFromFilename(const String &filename) noexcept
    {
        String point(".");

        ui64 pos = 0;
        for(pos = filename.length()-1; pos > 0; --pos)
        {
            if(filename.at(pos) == '.')
                break;
        }
        if(pos == 0)
            return Image::None;

        const auto len = filename.length() - pos;
        const auto str = filename.substr(pos, len);

        /// Doing a bunch of string compares :[
        if(str == ".png")
            return Image::Png;
        else if(str == ".jpg" || str == ".jpeg")
            return Image::Jpg;
        else if(str == ".bmp")
            return Image::Bmp;
        else if(str == ".webp")
            return Image::Webp;
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