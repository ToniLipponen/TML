#include <TML/Graphics/Font.h>
#include <TML/System/File.h>
#include <TML/TMLAssert.h>

#define STB_RECT_PACK_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_rect_pack.h>
#include <stb/stb_truetype.h>
#include <cstring>
#include <vector>

#include "Headers/TextDefines.h"

namespace tml
{
    Font::Font() noexcept
    : m_cdata(new stbtt_packedchar[FONT_MAX_GLYPH_COUNT])
    {

    }

    Font::Font(const Font& other) noexcept
    : m_cdata(new stbtt_packedchar[FONT_MAX_GLYPH_COUNT])
    {
        std::memcpy(m_cdata, other.m_cdata, sizeof(stbtt_packedchar) * FONT_MAX_GLYPH_COUNT);
        m_texture = other.m_texture;
    }

    Font::Font(Font&& other) noexcept
    : m_cdata(nullptr)
    {
        std::swap(m_cdata, other.m_cdata);
        m_texture = std::move(other.m_texture);
        m_kerningMap = std::move(other.m_kerningMap);
    }

    Font::~Font() noexcept
    {
        delete[] ((stbtt_packedchar*)m_cdata);
    }

    Font& Font::operator=(const Font& rhs) noexcept
    {
        if(&rhs == this)
        {
            return *this;
        }

        std::memcpy(m_cdata, rhs.m_cdata, sizeof(stbtt_packedchar) * FONT_MAX_GLYPH_COUNT);
        m_texture = rhs.m_texture;
        return *this;
    }

    Font& Font::operator=(Font&& rhs) noexcept
    {
        if(&rhs == this)
        {
            return *this;
        }

        delete[] ((stbtt_packedchar*)m_cdata);
        std::swap(m_cdata, rhs.m_cdata);
        m_texture = std::move(rhs.m_texture);
        m_kerningMap = std::move(rhs.m_kerningMap);
        return *this;
    }

    bool Font::LoadFromFile(const String& filename) noexcept
    {
        if(auto data = File::GetBytes(filename.cpp_str()))
        {
            return LoadFromData(reinterpret_cast<const uint8_t *>(data->data()));
        }

        return false;
    }

    bool Font::LoadFromData(const uint8_t* data) noexcept
    {
        if(!data)
        {
            return false;
        }

        int32_t size = 64;
        stbtt_pack_context context{};
        std::vector<uint8_t> bitmap;

        do
        {
            size *= 2;
            bitmap.resize(static_cast<uint64_t>(size) * size);

            if(!stbtt_PackBegin(&context, bitmap.data(), size, size, 0, 10, nullptr))
            {
                return false;
            }

            stbtt_PackSetOversampling(&context, FONT_OVER_SAMPLING, FONT_OVER_SAMPLING);
            stbtt_PackSetSkipMissingCodepoints(&context, 1);

            if(stbtt_PackFontRange(&context, data, 0, FONT_GLYPH_SIZE, 32, FONT_MAX_GLYPH_COUNT, (stbtt_packedchar*)m_cdata))
            {
                stbtt_PackEnd(&context);
                break;
            }

            stbtt_PackEnd(&context);
        } while(size < FONT_ATLAS_SIZE);

        m_texture.LoadFromMemory(size, size, 1, 8, bitmap.data());
        return MakeKerningTable(data);
    }

    void Font::GetAlignedQuad(void *output, int codePoint, float& x, float& y) noexcept
    {
        stbtt_GetPackedQuad(static_cast<const stbtt_packedchar*>(m_cdata),
                            m_texture.GetWidth(),
                            m_texture.GetHeight(),
                            codePoint,
                            &x,
                            &y,
                            static_cast<stbtt_aligned_quad*>(output), 1);
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

    uint32_t Font::GetAtlasSize() const noexcept
    {
        return m_texture.GetWidth();
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
