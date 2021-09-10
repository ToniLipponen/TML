#include "../include/TML/Font.h"
#include "internal/Assert.h"
#define STB_TRUETYPE_IMPLEMENTATION 1
#include "../external-headers/stb/stb_truetype.h"
#include "../external-headers/stb/stb_image_write.h"
#include <fstream>

#define ATLAS_SIZE 2048
using namespace tml;

Font::Font()
{
    m_cdata = new stbtt_bakedchar[96];
    m_font_info = new stbtt_fontinfo;
}

Font::Font(const std::string& filename)
{
    m_cdata = new stbtt_bakedchar[96];
    m_font_info = new stbtt_fontinfo;
    LoadFromFile(filename);
}

Font::Font(const ui8* data, ui32 datasize)
{
    m_cdata = new stbtt_bakedchar[96];
    m_font_info = new stbtt_fontinfo;
    LoadFromMemory(data, datasize);
}

// Todo: Make this call Font::LoadFromMemory to remove duplicate code
void Font::LoadFromFile(const std::string& filename)
{
    stbtt_bakedchar cdata[96];
    auto* bitmap = new unsigned char[ATLAS_SIZE*ATLAS_SIZE];
    unsigned char* buffer;
    stbtt_fontinfo font;
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if(!file.is_open())
    {
        Logger::ErrorMessage("Failed to load font -> %s", filename.c_str());
        delete[] bitmap;
        file.close();
        return;
    }
    const auto file_len = file.tellg();
    file.seekg(0, std::ios::beg);
    buffer = new unsigned char[file_len];
    file.read(reinterpret_cast<char*>(buffer), file_len);
    file.close();

    stbtt_InitFont(&font, buffer, 0);
    stbtt_BakeFontBitmap(reinterpret_cast<const unsigned char *>(buffer), 0, 385.0, bitmap, ATLAS_SIZE, ATLAS_SIZE, 32, 96, cdata);
    m_texture.LoadFromMemory(ATLAS_SIZE, ATLAS_SIZE, 1, bitmap);
    m_texture.SetClampMode(Texture::ClampToEdge);
    m_texture.SetMinMagFilter(Texture::Filter::Linear,
                              Texture::Filter::Linear);
    delete[] buffer;
}

void Font::LoadFromMemory(const ui8* data, ui32 size)
{
    auto* bitmap = new unsigned char[ATLAS_SIZE*ATLAS_SIZE];
    stbtt_InitFont((stbtt_fontinfo*)m_font_info, data, 0);
    stbtt_BakeFontBitmap(data, 0, 256.0, bitmap, ATLAS_SIZE, ATLAS_SIZE, 32, 96, (stbtt_bakedchar*)m_cdata);
    m_texture.LoadFromMemory(ATLAS_SIZE, ATLAS_SIZE, 1, bitmap);
    m_texture.SetClampMode(Texture::ClampToEdge);
    m_texture.SetMinMagFilter(Texture::Filter::LinearMipmapLinear, Texture::Filter::LinearMipmapLinear);
}