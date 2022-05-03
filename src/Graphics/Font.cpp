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
#define MAX_GLYPH_COUNT 1024

namespace tml
{
    Font::Font()
    {
        m_cdata = new stbtt_packedchar[MAX_GLYPH_COUNT];
    }

    Font::Font(const Font& rhs)
    {
        m_cdata = new stbtt_packedchar[MAX_GLYPH_COUNT];
        std::memcpy(m_cdata, rhs.m_cdata, sizeof(stbtt_packedchar) * MAX_GLYPH_COUNT);
        m_texture = rhs.m_texture;
    }

    Font::~Font()
    {
        delete[] ((stbtt_packedchar*)m_cdata);
    }

    Font& Font::operator=(const Font& rhs)
    {
        if(&rhs == this)
            return *this;

        std::memcpy(m_cdata, rhs.m_cdata, sizeof(stbtt_packedchar) * MAX_GLYPH_COUNT);
        m_texture = rhs.m_texture;
        return *this;
    }

    Font& Font::operator=(Font&& rhs) noexcept
    {
        if(&rhs == this)
            return *this;

        delete[] ((stbtt_packedchar*)m_cdata);
        std::swap(m_cdata, rhs.m_cdata);
        m_texture = std::move(rhs.m_texture);
        m_kerningMap = std::move(rhs.m_kerningMap);
        return *this;
    }

    bool Font::LoadFromFile(const String& filename) noexcept
    {
        std::vector<char> buffer;
        InFile file;
        if(!file.Open(filename))
            return false;

        file.GetBytes(buffer);
        return LoadFromData(reinterpret_cast<const uint8_t *>(buffer.data()));
    }

    bool Font::LoadFromData(const uint8_t* data) noexcept
    {
        if(!data)
        {
            return false;
        }

        std::unique_ptr<uint8_t[]> bitmap(new uint8_t[ATLAS_SIZE * ATLAS_SIZE]);
        stbtt_pack_context context{};

        if(!stbtt_PackBegin(&context, bitmap.get(), ATLAS_SIZE, ATLAS_SIZE, 0, 10, nullptr))
        {
            return false;
        }

        stbtt_PackSetOversampling(&context, OVER_SAMPLING, OVER_SAMPLING);
        stbtt_PackSetSkipMissingCodepoints(&context, 1);

        if(stbtt_PackFontRange(&context, data, 0, GLYPH_SIZE, 32, MAX_GLYPH_COUNT, (stbtt_packedchar*)m_cdata))
        {
            stbtt_PackEnd(&context);
            return false;
        }

        stbtt_PackEnd(&context);

        m_texture.LoadFromMemory(ATLAS_SIZE, ATLAS_SIZE, 1, bitmap.get());

        return MakeKerningTable(data);
    }

    void Font::GetAlignedQuad(void *output, int codePoint, float& x, float& y)
    {
        stbtt_GetPackedQuad(static_cast<const stbtt_packedchar*>(m_cdata), 4096, 4096, codePoint, &x, &y, static_cast<stbtt_aligned_quad*>(output), 0);
    }

    float Font::GetKerning(const std::pair<CodePoint, CodePoint>& pair) const noexcept
    {
        if(m_kerningMap.find(pair) != m_kerningMap.end())
        {
            return static_cast<float>(m_kerningMap.at(pair));
        }

        return 0;
    }

    float Font::GetKerning(CodePoint a, CodePoint b) const noexcept
    {
        const std::pair<int, int> pair = {a,b};

        if(m_kerningMap.find(pair) != m_kerningMap.end())
        {
            return static_cast<float>(m_kerningMap.at(pair));
        }

        return 0;
    }

    bool Font::MakeKerningTable(const uint8_t* data, int offset) noexcept
    {
        stbtt_fontinfo fontInfo{};
        stbtt_InitFont(&fontInfo, data, offset);
        const int tableLength = stbtt_GetKerningTableLength(&fontInfo);

        std::unique_ptr<stbtt_kerningentry[]> kerningTable(new stbtt_kerningentry[tableLength]);
        const int readLength = stbtt_GetKerningTable(&fontInfo, kerningTable.get(), tableLength);

        for(int i = 0; i < readLength; i++)
        {
            std::pair<int,int> pair = {kerningTable[i].glyph1, kerningTable[i].glyph2};
            m_kerningMap[pair] = kerningTable[i].advance;
        }

        return true; //!< Currently just returning true here, because the kerning table is optional anyway.
    }
}
