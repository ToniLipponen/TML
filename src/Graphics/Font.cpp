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
#define OVER_SAMPLING 2
#define GLYPH_SIZE 96

namespace tml
{
    Font::Font()
    {
        m_cdata = new stbtt_packedchar[1024];
    }

    Font::Font(const Font& rhs)
    {
        m_cdata = new stbtt_packedchar[1024];
        std::memcpy(m_cdata, rhs.m_cdata, sizeof(stbtt_packedchar) * 1024);
        m_texture = rhs.m_texture;
    }

    Font::~Font()
    {
        delete[] ((stbtt_packedchar*)m_cdata);
    }

    Font& Font::operator=(const Font& rhs)
    {
        std::memcpy(m_cdata, rhs.m_cdata, sizeof(stbtt_packedchar) * 1024);
        m_texture = rhs.m_texture;
        return *this;
    }

    void Font::LoadFromFile(const String& filename)
    {
        std::vector<char> buffer;
        InFile file;
        if(!file.Open(filename))
            return;
        file.GetBytes(buffer);
        LoadFromData(reinterpret_cast<const uint8_t *>(buffer.data()));
    }

    void Font::LoadFromData(const uint8_t* data)
    {
        auto* bitmap = new uint8_t[ATLAS_SIZE * ATLAS_SIZE];

        stbtt_pack_context context;
        stbtt_PackBegin(&context, bitmap, ATLAS_SIZE, ATLAS_SIZE, 0, 10, nullptr);
        stbtt_PackSetOversampling(&context, OVER_SAMPLING, OVER_SAMPLING);
        stbtt_PackSetSkipMissingCodepoints(&context, 1);
        stbtt_PackFontRange(&context, data, 0, GLYPH_SIZE, 32, 1024, (stbtt_packedchar*)m_cdata);
        stbtt_PackEnd(&context);

        m_texture.LoadFromMemory(ATLAS_SIZE, ATLAS_SIZE, 1, bitmap);
        delete[] bitmap;
    }

    void Font::GetAlignedQuad(void *output, int codePoint, float& x, float& y)
    {
        stbtt_GetPackedQuad(static_cast<const stbtt_packedchar*>(m_cdata), 4096, 4096, codePoint, &x, &y, static_cast<stbtt_aligned_quad*>(output), 0);
    }
}
