#include <TML/Graphics/Drawable/Text.h>
#include <TML/Utilities/Utilities.h>
#include <stb/stb_truetype.h>

tml::Text* DEFAULT_TEXT;
tml::Font* DEFAULT_FONT;

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
        if(font == "")
            m_font = *DEFAULT_FONT;
        else
            m_font.LoadFromFile(font);
        Generate();
    }

    void Text::SetFont(const Font &font)
    {
        m_font = font;
        Generate();
    }

    void Text::SetSpacing(ui32 s)
    {
        m_lineSpacing = s;
        Generate();
    }

    // First normalizes the quad coordinates, then scales them to size and translates them to xy.
    inline constexpr void NormalizeQuad(stbtt_aligned_quad& q, double s, double x, double y) noexcept
    {
        (q.x0 = (q.x0 * (s / 64.0))) += x;
        (q.x1 = (q.x1 * (s / 64.0))) += x;
        (q.y0 = (q.y0 * (s / 64.0))) += y - (s / 3.0);
        (q.y1 = (q.y1 * (s / 64.0))) += y - (s / 3.0);
    }

    void Text::Generate() noexcept
    {
        m_dimensions = Vector2f{0, m_size.y};
        float x = 0, y = 64.f;
        float width = 0, height = 0;
        ui32 count = 0;
        m_vertexData.clear();
        m_indexData.clear();
        const ui32 hex = m_color.Hex();
        for(auto c : m_string)
        {
            stbtt_aligned_quad q;
            if(c == '\n')
            {
                m_dimensions.x = Math::Max(width, m_dimensions.x);
                m_dimensions.y = Math::Max(height, m_dimensions.y);
                y += 64.f;
                x = 0;
                width = 0;
                continue;
            }
            stbtt_GetBakedQuad((stbtt_bakedchar*)m_font.m_cdata, 1024, 1024,int(c-32), &x, &y,&q, 1);
            NormalizeQuad(q, m_size.x, m_pos.x, m_pos.y);
            m_vertexData.push_back({{ceilf(q.x0), floorf(q.y0)}, {q.s0, q.t0}, hex, 0, Vertex::TEXT});
            m_vertexData.push_back({{ceilf(q.x1), floorf(q.y0)}, {q.s1, q.t0}, hex, 0, Vertex::TEXT});
            m_vertexData.push_back({{ceilf(q.x0), floorf(q.y1)}, {q.s0, q.t1}, hex, 0, Vertex::TEXT});
            m_vertexData.push_back({{ceilf(q.x1), floorf(q.y1)}, {q.s1, q.t1}, hex, 0, Vertex::TEXT});

            m_indexData.push_back(count + 0);
            m_indexData.push_back(count + 1);
            m_indexData.push_back(count + 2);

            m_indexData.push_back(count + 1);
            m_indexData.push_back(count + 3);
            m_indexData.push_back(count + 2);

            count += 4;
            width = q.x1 - m_pos.x;
            height = q.y1 - m_pos.y;
        }
        m_dimensions.x = Math::Max(width, m_dimensions.x);
        m_dimensions.y = Math::Max(height, m_dimensions.y);
    }
}