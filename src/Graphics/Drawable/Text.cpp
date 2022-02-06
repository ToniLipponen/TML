#include <TML/Graphics/Drawable/Text.h>
#include <TML/Utilities/Utilities.h>
#include <stb/stb_truetype.h>
#include <Font.h>

namespace tml
{
    Text::Text()
    {
        m_color = {255,255,255};
        m_pos = Vector2f{0,0};
        m_size = Vector2f{32,32};
    }

    void Text::SetSize(float s)
    {
        m_size = Vector2f{s,s};
        Generate();
    }

    void Text::SetString(const std::string& string, const std::string& font)
    {
        SetString(Util::StringToWstring(string), font);
    }

    void Text::SetString(const std::wstring& string, const std::string& font)
    {
        m_string = string;
        if(!m_hasFont)
        {
            if(font == "")
            {
                m_font.LoadFromMemory(DEFAULT_FONT.data(), DEFAULT_FONT.size());
            }
            else
                m_font.LoadFromFile(font);
            m_hasFont = true;
        }
        Generate();
    }

    void Text::SetFont(const Font &font)
    {
        m_font = font;
        m_hasFont = true;
        Generate();
    }

    void Text::SetSpacing(float s)
    {
        m_lineSpacing = s;
        Generate();
    }

    void Text::SetKerning(float s)
    {
        m_kerning = s;
        Generate();
    }

    // First normalizes the quad coordinates, then scales them to size and translates them to xy.
    inline constexpr void NormalizeQuad(stbtt_aligned_quad& q, double s, double x, double y) noexcept
    {
        q.x1 = ((q.x1 * (s / 128.0)) + x);
        q.x0 = ((q.x0 * (s / 128.0)) + x);
        q.y0 = ((q.y0 * (s / 128.0)) + y);
        q.y1 = ((q.y1 * (s / 128.0)) + y);
    }

    void Text::Generate() noexcept
    {
        m_dimensions = Vector2f{0, m_size.y};
        float x = 0, y = 96;
        float width = 0, height = 0;
        ui32 count = 0;
        m_vertexData.clear();
        m_indexData.clear();
        const ui32 hex = m_color.Hex();

        for(auto c : m_string)
        {
            if(c == '\n')
            {
                m_dimensions.x = Math::Max(width, m_dimensions.x);
                m_dimensions.y = Math::Max(height, m_dimensions.y);
                y += 96 + m_lineSpacing;
                x = 0;
                width = 0;
                continue;
            }

            stbtt_aligned_quad q;
            stbtt_GetPackedQuad(((const stbtt_packedchar *)m_font.m_cdata), 2048, 2048,int(c-32),&x,&y,&q,1);
            NormalizeQuad(q, m_size.x, m_pos.x, m_pos.y);

            q.x0 = roundf(q.x0);
            q.x1 = roundf(q.x1);
            q.y0 = roundf(q.y0);
            q.y1 = roundf(q.y1);

            m_vertexData.push_back({{q.x0, q.y0}, {q.s0, q.t0}, hex, 0, Vertex::TEXT});
            m_vertexData.push_back({{q.x1, q.y0}, {q.s1, q.t0}, hex, 0, Vertex::TEXT});
            m_vertexData.push_back({{q.x0, q.y1}, {q.s0, q.t1}, hex, 0, Vertex::TEXT});
            m_vertexData.push_back({{q.x1, q.y1}, {q.s1, q.t1}, hex, 0, Vertex::TEXT});

            m_indexData.push_back(count + 0);
            m_indexData.push_back(count + 1);
            m_indexData.push_back(count + 2);

            m_indexData.push_back(count + 1);
            m_indexData.push_back(count + 3);
            m_indexData.push_back(count + 2);

            count += 4;
            width = q.x1 - m_pos.x;
            height = q.y1 - m_pos.y;
            x += m_kerning;
        }
        m_dimensions.x = Math::Max(width, m_dimensions.x);
        m_dimensions.y = Math::Max(height, m_dimensions.y);
    }
}