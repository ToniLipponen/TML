#include <TML/Graphics/Font.h>
#include <TML/System/File.h>
#include "../Headers/_Assert.h"
#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_rect_pack.h>
#include <stb/stb_truetype.h>
#include <cstring>
#include <vector>

#define ATLAS_SIZE 4096
#define OVER_SAMPLING 4
#define GLYPH_SIZE ATLAS_SIZE / 16.0 / OVER_SAMPLING

#define FILTER_MODE Texture::LinearMipmapLinear
namespace tml
{
    Font::Font()
    {
        m_cdata = new stbtt_packedchar[512];
        m_bitmap = new ui8[ATLAS_SIZE*ATLAS_SIZE];
        m_texture.SetMinMagFilter(FILTER_MODE, FILTER_MODE);
    }

    Font::Font(const Font& rhs)
    {
        m_cdata = new stbtt_packedchar[512];
        m_bitmap = new ui8[ATLAS_SIZE*ATLAS_SIZE];
        std::memcpy(m_cdata, rhs.m_cdata, sizeof(stbtt_packedchar) * 512);
        std::memcpy(m_bitmap, rhs.m_bitmap, ATLAS_SIZE*ATLAS_SIZE);

        m_texture.LoadFromMemory(ATLAS_SIZE, ATLAS_SIZE, 1, m_bitmap);
        m_texture.SetMinMagFilter(FILTER_MODE, FILTER_MODE);
    }

    Font::~Font()
    {
        delete[] ((stbtt_packedchar*)m_cdata);
        delete[] m_bitmap;
    }

    Font& Font::operator=(const Font& rhs)
    {
        std::memcpy(m_cdata, rhs.m_cdata, sizeof(stbtt_packedchar) * 512);
        std::memcpy(m_bitmap, rhs.m_bitmap, ATLAS_SIZE*ATLAS_SIZE);
        m_texture.LoadFromMemory(ATLAS_SIZE, ATLAS_SIZE, 1, m_bitmap);
        return *this;
    }

    void Font::LoadFromFile(const std::string& filename)
    {
        std::vector<char> buffer;
        InFile file;
        if(!file.Open(filename))
            return;
        file.GetBytes(buffer);
        LoadFromMemory(reinterpret_cast<const ui8 *>(buffer.data()), buffer.size());
    }

    void Font::LoadFromMemory(const ui8* data, ui32 size)
    {
        stbtt_pack_context context;
        stbtt_PackBegin(&context, m_bitmap, ATLAS_SIZE, ATLAS_SIZE, 0, 1, nullptr);
        stbtt_PackSetOversampling(&context, OVER_SAMPLING, OVER_SAMPLING);
        stbtt_PackSetSkipMissingCodepoints(&context, 1);
        stbtt_PackFontRange(&context, data, 0, GLYPH_SIZE, 32, 512, (stbtt_packedchar*)m_cdata);
        stbtt_PackEnd(&context);
        m_texture.LoadFromMemory(ATLAS_SIZE, ATLAS_SIZE, 1, m_bitmap);
    }
}
