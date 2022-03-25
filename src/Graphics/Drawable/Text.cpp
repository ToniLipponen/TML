#include <TML/Graphics/Drawable/Text.h>
#include <TML/Graphics/Renderer.h>
#include <TML/System/Math.h>
#include <stb/stb_truetype.h>
#include <Font.h>

namespace tml
{
    Font* Text::s_defaultFont = nullptr;

    Text::Text()
    {
        m_color = Color{255,255,255,255};
        m_pos   = Vector2f{0,0};
        m_size  = Vector2f{32,32};
        m_font  = nullptr;
    }

    void Text::SetSize(float s) noexcept
    {
        m_updated = (m_size.x != s) || m_updated;
        m_size = Vector2f{s,s};
    }

    void Text::SetString(const String& string) noexcept
    {
        m_string = string;
        m_updated = true;
    }

    void Text::SetFont(const Font& font) noexcept
    {
        m_font = std::make_shared<Font>(font);
        m_updated = true;
    }

    void Text::SetFont(const std::shared_ptr<Font>& font) noexcept
    {
        m_font = font;
    }

    void Text::SetSpacing(float s) noexcept
    {
        m_updated = (m_lineSpacing != s) || m_updated;
        m_lineSpacing = s;
    }

    void Text::SetKerning(float s) noexcept
    {
        m_updated = (m_kerning != s) || m_updated;
        m_kerning = s;
    }

    Vector2f Text::GetDimensions() noexcept
    {
        Generate();
        return m_dimensions;
    }

    inline constexpr void NormalizeQuad(stbtt_aligned_quad& q, double s, double x, double y) noexcept
    {
        q.x1 = float((q.x1 * (s / 64.0)) + x);
        q.x0 = float((q.x0 * (s / 64.0)) + x);
        q.y0 = float((q.y0 * (s / 64.0)) + y);
        q.y1 = float((q.y1 * (s / 64.0)) + y);
    }

    void Text::Generate()
    {
        if(!s_defaultFont)
        {
            s_defaultFont = new Font;
            s_defaultFont->LoadFromMemory(TML_DEFAULT_FONT_DATA, TML_DEFAULT_FONT_LENGTH);
        }
        m_dimensions = Vector2f{0, m_size.y};
        float x = 0, y = 48;
        float width = 0, height = 0;
        ui32 count = 0;
        m_vertexData.clear();
        m_indexData.clear();
        const ui32 hex = m_color.Hex();
        Font& font = m_font ? *m_font : *s_defaultFont;

        for(auto c : m_string)
        {
            if(Math::InRange<i32>(c, 0, 8) || Math::InRange<i32>(c, 14, 31))
                continue;

            switch(c)
            {
                case 11:
                case 12:
                    break;
                case '\r':
                case '\n':
                    m_dimensions.x = Math::Max(width, m_dimensions.x);
                    m_dimensions.y = Math::Max(height+m_size.x+m_lineSpacing, m_dimensions.y);
                    y += 64.0 - (64.0 / 3.0) + m_lineSpacing;
                    x = 0;
                    width = 0;
                    break;

                case '\t':
                    x += m_size.x * 3 + m_kerning;
                    break;

                default:
                    stbtt_aligned_quad q;
                    stbtt_GetPackedQuad(((const stbtt_packedchar *)font.m_cdata), 4096, 4096, int(c-32), &x, &y, &q, 0);
                    NormalizeQuad(q, m_size.x, m_pos.x, m_pos.y);

                    q.x0 = roundf(q.x0);
                    q.x1 = roundf(q.x1);
                    q.y0 = roundf(q.y0);
                    q.y1 = roundf(q.y1);

                    m_vertexData.push_back({{q.x0, q.y0}, {q.s0, q.t0}, hex, Vertex::TEXT});
                    m_vertexData.push_back({{q.x1, q.y0}, {q.s1, q.t0}, hex, Vertex::TEXT});
                    m_vertexData.push_back({{q.x0, q.y1}, {q.s0, q.t1}, hex, Vertex::TEXT});
                    m_vertexData.push_back({{q.x1, q.y1}, {q.s1, q.t1}, hex, Vertex::TEXT});

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
                    break;
            }
        }
        m_dimensions.x = Math::Max(width, m_dimensions.x);
        m_dimensions.y = Math::Max(height, m_dimensions.y);
    }

    void Text::OnDraw(class Renderer* renderer, Texture*) noexcept
    {
        if(m_updated)
        {
            Generate();
            m_updated = false;
        }
        renderer->PushVertexData(m_vertexData, m_indexData, m_font ? m_font->m_texture : s_defaultFont->m_texture);
    }
}