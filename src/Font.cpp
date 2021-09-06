#include "../include/Font.h"
#define STB_TRUETYPE_IMPLEMENTATION 1
#include "../external-headers//stb/stb_truetype.h"
#include <fstream>

#define ATLAS_SIZE 4096
using namespace tml;

inline constexpr static void AddCharToAtlas(ui8* atlasData, ui8* charData, ui32 w, ui32 h, ui32 x, ui32 y)
{
    for(int i = y; i < y+h; ++i)
    {
        for(int j = x; j < x+w; ++j)
        {
            atlasData[i*ATLAS_SIZE+j] = charData[(i-y)*w + (j-x)];
        }
    }
}

Font::Font(){}

Font::Font(const std::string& filename)
{
    LoadFromFile(filename);
}

Font::Font(const ui8* data, ui32 datasize)
{
    LoadFromMemory(data, datasize);
}

// Todo: Make this call Font::LoadFromMemory to remove duplicate code
void Font::LoadFromFile(const std::string& filename)
{
    m_chars.clear();
    stbtt_bakedchar cdata[96];
    unsigned char* bitmap = new unsigned char[ATLAS_SIZE*ATLAS_SIZE];
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
    for(char i = 0; i < 96; i++)
    {
        m_chars.insert({i+32,FontChar{
            {static_cast<float>(cdata[i].x1) / ATLAS_SIZE, static_cast<float>(cdata[i].y1) / ATLAS_SIZE},
            {static_cast<float>(cdata[i].x0) / ATLAS_SIZE, static_cast<float>(cdata[i].y0) / ATLAS_SIZE},
            {static_cast<float>(cdata[i].xoff) / ATLAS_SIZE, static_cast<float>(cdata[i].yoff) / ATLAS_SIZE},
            cdata[i].xadvance}});
    }
    delete[] buffer;
}

void Font::LoadFromMemory(const ui8* data, ui32 size)
{
    m_chars.clear();
    stbtt_bakedchar cdata[96];
    unsigned char* bitmap = new unsigned char[ATLAS_SIZE*ATLAS_SIZE];
    stbtt_fontinfo font;

    stbtt_InitFont(&font, data, 0);
    stbtt_BakeFontBitmap(data, 0, 385.0, bitmap, ATLAS_SIZE, ATLAS_SIZE, 32, 96, cdata);
    m_texture.LoadFromMemory(ATLAS_SIZE, ATLAS_SIZE, 1, bitmap);
    m_texture.SetClampMode(Texture::ClampToEdge);
    m_texture.SetMinMagFilter(Texture::Filter::Linear, Texture::Filter::Linear);
    for(int i = 0; i < 96; i++)
    {
        m_chars[static_cast<char>(i+32)] = FontChar{
                {static_cast<float>(cdata[i].x1) / ATLAS_SIZE, static_cast<float>(cdata[i].y1) / ATLAS_SIZE},
                {static_cast<float>(cdata[i].x0) / ATLAS_SIZE, static_cast<float>(cdata[i].y0) / ATLAS_SIZE},
                {static_cast<float>(cdata[i].xoff) / ATLAS_SIZE, static_cast<float>(cdata[i].yoff) / ATLAS_SIZE},
                cdata[i].xadvance};
    }
}