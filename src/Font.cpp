#include <TML/Font.h>
#include "internal/Assert.h"
#define STB_TRUETYPE_IMPLEMENTATION 1
#include <stb/stb_truetype.h>
#include <stb/stb_image_write.h>
#include <fstream>

#define ATLAS_SIZE 1024
using namespace tml;

Font::Font()
{
    m_cdata = new stbtt_bakedchar[96];
    m_font_info = new stbtt_fontinfo;
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
    auto* bitmap = new unsigned char[ATLAS_SIZE*ATLAS_SIZE];
    stbtt_InitFont((stbtt_fontinfo*)m_font_info, data, 0);
    stbtt_BakeFontBitmap(data, 0, 128.0, bitmap, ATLAS_SIZE, ATLAS_SIZE, 32, 96, (stbtt_bakedchar*)m_cdata);
    m_texture.LoadFromMemory(ATLAS_SIZE, ATLAS_SIZE, 1, bitmap);
    stbi_write_png("font.png",ATLAS_SIZE, ATLAS_SIZE, 1, bitmap, 0);
    delete[] bitmap;
}