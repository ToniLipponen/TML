#include <TML/Graphics/Font.h>
#include "_Assert.h"

#define STB_RECT_PACK_IMPLEMENTATION 1
#define STB_TRUETYPE_IMPLEMENTATION 1
#include <stb/stb_rect_pack.h>
#include <stb/stb_truetype.h>

#include <TML/Graphics/Image.h>
#include <fstream>
#include <cstring>

#define ATLAS_SIZE 4096
#define OVER_SAMPLING 4
#define GLYPH_SIZE ATLAS_SIZE / 16.0 / OVER_SAMPLING
using namespace tml;

Font::Font()
{
    m_cdata = new stbtt_packedchar[512];
}

Font::Font(const Font& rhs)
{
    m_cdata = new stbtt_packedchar[512];
    std::memcpy(m_cdata, rhs.m_cdata, sizeof(stbtt_packedchar) * 512);
}

Font::~Font()
{
    delete[] ((stbtt_packedchar*)m_cdata);
}

Font& Font::operator=(const Font& rhs)
{
    std::memcpy(m_cdata, rhs.m_cdata, sizeof(stbtt_aligned_quad) * 512);
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
    stbtt_pack_context context;
    stbtt_PackBegin(&context, m_image.GetData(), ATLAS_SIZE, ATLAS_SIZE, 0, 1, nullptr);
    stbtt_PackSetOversampling(&context, OVER_SAMPLING, OVER_SAMPLING);
    stbtt_PackSetSkipMissingCodepoints(&context, 1);
    stbtt_PackFontRange(&context, data, 0, GLYPH_SIZE, 32, 512, (stbtt_packedchar*)m_cdata);
    stbtt_PackEnd(&context);
    m_texture.SetMinMagFilter(Texture::Nearest, Texture::Linear);
    m_texture.LoadFromImage(m_image);
}