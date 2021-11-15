#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#include <TML/Graphics/Image.h>
#include <webp/decode.h>
#include <webp/encode.h>
#include <TML/IO/File.h>

namespace tml
{
    Image::Image()
    : m_width(0), m_height(0), m_Bpp(0), m_data(nullptr)
    {

    }

    Image::Image(i32 w, i32 h, i32 Bpp, const ui8* data)
    : m_width(w), m_height(h), m_Bpp(Bpp), m_data(nullptr)
    {
        m_data = new ui8[w*h*Bpp];
        memcpy(m_data, data, w*h*Bpp);
    }

    Image::Image(const ui8* data, ui32 s)
    : m_width(0), m_height(0), m_Bpp(0), m_data(nullptr)
    {
        LoadFromData(data, s);
    }

    Image::Image(const std::string& fileName)
    : m_width(0), m_height(0), m_Bpp(0), m_data(nullptr)
    {
        LoadFromFile(fileName);
    }

    Image::Image(const Image& image)
    : m_width(0), m_height(0), m_Bpp(0), m_data(nullptr)
    {
        this->m_width = image.m_width;
        this->m_height = image.m_height;
        this->m_Bpp = image.m_Bpp;

        delete[] m_data;
        m_data = new ui8[m_width*m_height*m_Bpp];
        memcpy(m_data, image.m_data, m_width*m_height*m_Bpp);
    }

    Image::Image(Image&& image) noexcept
    : m_width(0), m_height(0), m_Bpp(0), m_data(nullptr)
    {
        *this = image;
    }

    Image::~Image()
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

        // Reallocate memory only if the dimensions and bytes per pixel of the images don't match.
        if(m_width != rhs.m_width || m_height != rhs.m_height || m_Bpp != rhs.m_Bpp)
        {
            delete[] m_data;
            m_data = new ui8[m_width * m_height * m_Bpp];
        }
        memcpy(m_data, rhs.m_data, m_width * m_height * m_Bpp);
        return *this;
    }

    Image& Image::operator=(Image&& rhs) noexcept
    {
        this->m_width = rhs.m_width;
        this->m_height = rhs.m_height;
        this->m_Bpp = rhs.m_Bpp;
        delete[] m_data;

        this->m_data = rhs.m_data;
        rhs.m_data = nullptr;
        rhs.m_width = 0;
        rhs.m_height = 0;
        rhs.m_Bpp = 0;
        return *this;
    }

    bool Image::LoadFromFile(const std::string& fileName)
    {
        delete[] m_data; m_data = nullptr;
        const auto imageType = GetTypeFromFilename(fileName);
        if(imageType == Image::Webp)
        {
            if(!LoadWebp(fileName))
                return false;
        }
        else
            m_data = stbi_load(fileName.c_str(), &m_width, &m_height, &m_Bpp, 0);
        return (m_data != nullptr);
    }

    void Image::LoadFromMemory(i32 w, i32 h, i32 Bpp, const ui8* data)
    {
        delete[] m_data;
        m_data = new ui8[w*h*Bpp];
        memcpy(m_data, data, w*h*Bpp);
        m_width = w;
        m_height = h;
        m_Bpp = Bpp;
    }

    bool Image::LoadFromData(const ui8 *data, ui32 dataSize)
    {
        delete[] m_data; m_data = nullptr;
        m_data = stbi_load_from_memory(data, static_cast<int>(dataSize), &m_width, &m_height, &m_Bpp, 0);
        if(m_data == nullptr)
            return LoadWebp(data, dataSize);

        return false;
    }

    bool Image::SaveToFile(const std::string& fileName, int quality)
    {
        const auto type = GetTypeFromFilename(fileName);

        if(type == Image::Png)
            return stbi_write_png(fileName.c_str(), m_width, m_height, m_Bpp, m_data, 0) != 0;
        else if(type == Image::Bmp)
            return stbi_write_bmp(fileName.c_str(), m_width, m_height, m_Bpp, m_data) != 0;
        else if(type == Image::Tga)
            return stbi_write_tga(fileName.c_str(), m_width, m_height, m_Bpp, m_data) != 0;
        else if(type == Image::Webp)
            return SaveWebp(fileName, quality);
        else // Default to jpg
            return stbi_write_jpg(fileName.c_str(), m_width, m_height, m_Bpp, m_data, quality) != 0;
    }

    bool Image::LoadWebp(const std::string &filename)
    {
        InFile file;
        file.Open(filename);
        const auto data = file.GetBytes();

       return LoadWebp(reinterpret_cast<const ui8*>(data.data()), data.size());
    }

    bool Image::LoadWebp(const ui8 *data, ui32 size)
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

    bool Image::SaveWebp(const std::string& filename, int quality)
    {
        ui8* output = nullptr;
        ui64 size = 0;

        if(m_Bpp == 3)
            size = WebPEncodeRGB(m_data, m_width, m_height, m_width*m_Bpp, static_cast<float>(quality), &output);
        else if(m_Bpp == 4)
            size = WebPEncodeRGBA(m_data, m_width, m_height, m_width*m_Bpp, static_cast<float>(quality), &output);

        if(size && output)
        {
            OutFile file;
            file.Open(filename);
            file.Write(reinterpret_cast<const char *>(output), size);
            return true;
        }
        return false;
    }

    Image::ImageType Image::GetTypeFromFilename(const std::string &filename)
    {
        const auto str = filename.substr(filename.find_last_of('.'), filename.length());

        if(str == ".png")
            return Image::Png;
        else if(str == ".bmp")
            return Image::Bmp;
        else if(str == ".tga")
            return Image::Tga;
        else if(str == ".jpg" || str == ".jpeg")
            return Image::Jpg;
        else if(str == ".pic")
            return Image::Pic;
        else if(str == ".ppm")
            return Image::Pnm;
        else if(str == ".webp")
            return Image::Webp;

        return Image::None;
    }

}