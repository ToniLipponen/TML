#include <TML/Graphics/Font.h>
#include <TML/System/File.h>
#include <TML/TMLAssert.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>
#include <cstring>
#include <vector>

#include "Headers/TextDefines.h"

namespace tml
{
    inline void CopyPixels(
        unsigned char* dest, 
        unsigned char* src, 
        int destW, 
        int destH, 
        int srcW, 
        int srcH, 
        int posX, 
        int posY)
    {
        for(int y = posY; y < posY + srcH; y++)
        {
            int srcY = y - posY;
            std::memcpy(dest + y * destW + posX, src + srcY * srcW, srcW);
        }
    }

    Font::Font() noexcept
    : m_charData(new stbtt_bakedchar[FONT_MAX_GLYPH_COUNT]), m_alignedQuads(new stbtt_aligned_quad[FONT_MAX_GLYPH_COUNT])
    {

    }

    Font::Font(const Font& other) noexcept
    : m_charData(new stbtt_bakedchar[FONT_MAX_GLYPH_COUNT]), m_alignedQuads(new stbtt_aligned_quad[FONT_MAX_GLYPH_COUNT])
    {
        std::memcpy(m_charData, other.m_charData, sizeof(stbtt_bakedchar) * FONT_MAX_GLYPH_COUNT);
        std::memcpy(m_alignedQuads, other.m_alignedQuads, sizeof(stbtt_aligned_quad) * FONT_MAX_GLYPH_COUNT);
        m_texture = other.m_texture;
        m_kerningMap = other.m_kerningMap;
    }

    Font::Font(Font&& other) noexcept
    {
        std::swap(m_charData, other.m_charData);
        std::swap(m_alignedQuads, other.m_alignedQuads);
        m_texture = std::move(other.m_texture);
        m_kerningMap = std::move(other.m_kerningMap);
    }

    Font::~Font() noexcept
    {
        delete[] ((stbtt_bakedchar*)m_charData);
        delete[] ((stbtt_aligned_quad*)m_alignedQuads);
    }

    Font& Font::operator=(const Font& rhs) noexcept
    {
        if(&rhs == this)
        {
            return *this;
        }

        std::memcpy(m_charData, rhs.m_charData, sizeof(stbtt_bakedchar) * FONT_MAX_GLYPH_COUNT);
        std::memcpy(m_alignedQuads, rhs.m_alignedQuads, sizeof(stbtt_aligned_quad) * FONT_MAX_GLYPH_COUNT);
        m_texture = rhs.m_texture;
        m_kerningMap = rhs.m_kerningMap;

        return *this;
    }

    Font& Font::operator=(Font&& rhs) noexcept
    {
        if(&rhs == this)
        {
            return *this;
        }

        delete[] ((stbtt_bakedchar*)m_charData);
        delete[] ((stbtt_aligned_quad*)m_alignedQuads);
        std::swap(m_charData, rhs.m_charData);
        std::swap(m_alignedQuads, rhs.m_alignedQuads);
        m_texture = std::move(rhs.m_texture);
        m_kerningMap = std::move(rhs.m_kerningMap);

        return *this;
    }

    bool Font::LoadFromFile(const String& filename) noexcept
    {
        auto data = File::ReadBytes(filename.cpp_str());

        if(!data.empty())
        {
            return LoadFromData(reinterpret_cast<const uint8_t *>(data.data()));
        }

        return false;
    }

    bool Font::LoadFromData(const uint8_t* data) noexcept
    {
        if(!data)
        {
            return false;
        }

        std::vector<unsigned char> bitmap;
        bitmap.resize(FONT_ATLAS_SIZE * FONT_ATLAS_SIZE);
        
        stbtt_fontinfo info;
        stbtt_InitFont(&info, data, 0);

        float sdfSize = FONT_GLYPH_SIZE;
        float pixelDistScale = 16.0;
        int onedgeValue = 128;
        int padding = 3;

        auto scale = stbtt_ScaleForPixelHeight(&info, sdfSize);
        auto packed = (stbtt_aligned_quad*)m_alignedQuads;
        float xPos = 0, yPos = 0;

        for(int codePoint = 33; codePoint < FONT_MAX_GLYPH_COUNT; codePoint++)
        {
            if(stbtt_FindGlyphIndex(&info, codePoint) == 0)
            {
                continue;
            }

            int advance = 0, lsb;
            stbtt_GetCodepointHMetrics(&info, codePoint, &advance, &lsb);

            int w, h, x, y;
            auto pixels = stbtt_GetCodepointSDF(
                &info, scale, codePoint, padding, onedgeValue, pixelDistScale, &w, &h, &x, &y);
            
            if(pixels == nullptr)
            {
                continue;
            }

            if(xPos + w > FONT_ATLAS_SIZE)
            {
                yPos += static_cast<int>(sdfSize);
                xPos = 0;
            }

            stbtt_bakedchar& packedChar = static_cast<stbtt_bakedchar*>(m_charData)[codePoint];
            packedChar.x0 = xPos;
            packedChar.x1 = xPos + w;
            packedChar.y0 = yPos;
            packedChar.y1 = yPos + sdfSize;

            packedChar.yoff = sdfSize + y;
            packedChar.xoff = w + x;
            packedChar.xadvance = advance;
            
            CopyPixels(bitmap.data(), pixels, FONT_ATLAS_SIZE, FONT_ATLAS_SIZE, w, h, xPos, yPos);
            auto xOff = w + x;
            auto yOff = sdfSize + y;
            packed[codePoint].x0 = x;
            packed[codePoint].y0 = yOff;
            packed[codePoint].x1 = w + x;
            packed[codePoint].y1 = yOff + sdfSize;
            
            packed[codePoint].s0 = xPos / FONT_ATLAS_SIZE;
            packed[codePoint].s1 = (xPos + w) / FONT_ATLAS_SIZE;
            packed[codePoint].t0 = yPos / FONT_ATLAS_SIZE;
            packed[codePoint].t1 = (yPos + sdfSize) / FONT_ATLAS_SIZE;
            xPos += w;
        }

        m_texture.LoadFromMemory(FONT_ATLAS_SIZE, FONT_ATLAS_SIZE, 1, 8, bitmap.data());

        return MakeKerningTable(data);
    }

    void Font::GetAlignedQuad(void *output, int codePoint, float& x, float& y) noexcept
    {
        auto alignedQuadOut = static_cast<stbtt_aligned_quad*>(output);
        auto alignedQuads = static_cast<stbtt_aligned_quad*>(m_alignedQuads);
        auto alignedQuad = alignedQuads + codePoint;
        memcpy(output, alignedQuad, sizeof(stbtt_aligned_quad));
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
