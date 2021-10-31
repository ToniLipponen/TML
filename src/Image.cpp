#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#include <TML/Image.h>

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
    {
        LoadFromData(data, s);
    }

    Image::Image(const std::string& fileName)
    : m_width(0), m_height(0), m_Bpp(0), m_data(nullptr)
    {
        m_data = stbi_load(fileName.c_str(), &m_width, &m_height, &m_Bpp, 0);
    }

    Image::Image(const Image& image)
    {
        this->m_width = image.m_width;
        this->m_height = image.m_height;
        this->m_Bpp = image.m_Bpp;

        delete[] m_data;
        m_data = new ui8[m_width*m_height*m_Bpp];
        memcpy(m_data, image.m_data, m_width*m_height*m_Bpp);
    }

    Image::Image(Image&& image)
    {
        this->m_width = image.m_width;
        this->m_height = image.m_height;
        this->m_Bpp = image.m_Bpp;
        delete[] m_data;

        this->m_data = image.m_data;
        image.m_data = nullptr;
        image.m_width = 0;
        image.m_height = 0;
        image.m_Bpp = 0;
    }

    Image::~Image()
    {
        delete[] m_data;
    }

    Image& Image::operator=(const Image& rhs) noexcept
    {
        this->m_width = rhs.m_width;
        this->m_height = rhs.m_height;
        this->m_Bpp = rhs.m_Bpp;

        delete[] m_data;
        m_data = new ui8[m_width*m_height*m_Bpp];
        memcpy(m_data,rhs.m_data, m_width*m_height*m_Bpp);
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
        m_data = stbi_load_from_memory(data, dataSize, &m_width, &m_height, &m_Bpp, 0);
        return m_data != nullptr;
    }

    bool Image::SaveToFile(const std::string& fileName)
    {
        const auto ending = fileName.substr(fileName.find_last_of('.'), fileName.length());
        if(ending == "png")
            return stbi_write_png(fileName.c_str(), m_width, m_height, m_Bpp, m_data, 0) != 0;
        else if(ending == "bmp")
            return stbi_write_bmp(fileName.c_str(), m_width, m_height, m_Bpp, m_data) != 0;
        else if(ending == "tga")
            return stbi_write_tga(fileName.c_str(), m_width, m_height, m_Bpp, m_data) != 0;
        else // Default to jpg
            return stbi_write_jpg(fileName.c_str(), m_width, m_height, m_Bpp, m_data, 90) != 0;
    }
}