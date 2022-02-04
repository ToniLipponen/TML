#include <TML/Graphics/Font.h>
#include "_Assert.h"
#define STB_TRUETYPE_IMPLEMENTATION 1
#include <stb/stb_truetype.h>
#include <TML/Graphics/Image.h>
#include <fstream>
#include <cstring>

#define ATLAS_SIZE 1024
#define GLYPH_SIZE ATLAS_SIZE / 16.0
using namespace tml;

Font::Font()
{
    m_cdata = new stbtt_bakedchar[512];
    m_font_info = new stbtt_fontinfo;
}

Font::Font(const Font& rhs)
{
    m_cdata = new stbtt_bakedchar[512];
    m_font_info = new stbtt_fontinfo;

    std::memcpy(m_cdata, rhs.m_cdata, sizeof(stbtt_bakedchar) * 512);
    *((stbtt_fontinfo*)m_font_info) = *((stbtt_fontinfo*)rhs.m_font_info);
}

Font::~Font()
{
    delete[] ((stbtt_bakedchar*)m_cdata);
    delete ((stbtt_fontinfo*)m_font_info);
}

Font& Font::operator=(const Font& rhs)
{
    std::memcpy(m_cdata, rhs.m_cdata, sizeof(stbtt_bakedchar) * 512);
    *((stbtt_fontinfo*)m_font_info) = *((stbtt_fontinfo*)rhs.m_font_info);
    m_image = rhs.m_image;
    m_texture.LoadFromImage(m_image);
    return *this;
}

void Font::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if(!file.is_open())
    {
        Logger::ErrorMessage("Failed to load font -> %s", filename.c_str());
        file.close();
        return;
    }
    unsigned char* buffer;
    const auto file_len = file.tellg();
    file.seekg(0, std::ios::beg);
    buffer = new unsigned char[file_len];
    file.read((char*)buffer, file_len);
    file.close();
    LoadFromMemory(buffer, file_len);
    delete[] buffer;
}

void Font::LoadFromMemory(const ui8* data, ui32 size)
{
    m_image.LoadFromMemory(ATLAS_SIZE, ATLAS_SIZE, 1, nullptr);
    stbtt_InitFont((stbtt_fontinfo*)m_font_info, data, 0);
    stbtt_BakeFontBitmap(data, 0, GLYPH_SIZE, m_image.GetData(), ATLAS_SIZE, ATLAS_SIZE, 32, 512-32, (stbtt_bakedchar*)m_cdata);
    m_texture.LoadFromImage(m_image);
}