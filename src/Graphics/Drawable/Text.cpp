#include <TML/Graphics/Drawable/Text.h>
#include "../Renderer.h"
#include <TML/System/Math.h>
#include <TML/Graphics/Font.h>
#include "../../Headers/Font.h" //!< Default font data.

struct stbtt_aligned_quad
{
    float x0,y0,s0,t0; // top-left
    float x1,y1,s1,t1; // bottom-right
};

namespace tml
{
    std::unique_ptr<Font> Text::s_defaultFont = nullptr;

    Text::Text()
    : m_font(nullptr), m_string(""), m_lineSpacing(0), m_kerning(0), m_dimensions(0)
    {
        m_color = Color{255,255,255,255};
        m_pos   = Vector2f{0,0};
        m_size  = Vector2f{32,32};
    }

    void Text::SetSize(float s) noexcept
    {
        m_updated = (m_size.x != s) || m_updated;
        m_size = Vector2f{s,s};
    }

    void Text::SetString(const String& string) noexcept
    {
        if(string != m_string)
        {
            m_string = string;
            m_updated = true;
        }
    }

    void Text::SetFont(const Font& font) noexcept
    {
        m_font = std::make_shared<Font>(font);
        m_updated = true;
    }

    void Text::SetFont(const std::shared_ptr<Font>& font) noexcept
    {
        if(m_font != font)
        {
            m_font = font;
            m_updated = true;
        }
    }

    void Text::SetSpacing(float s) noexcept
    {
        if(m_lineSpacing != s)
        {
            m_updated = true;
            m_lineSpacing = s;
        }
    }

    void Text::SetKerning(float s) noexcept
    {
        if(m_kerning != s)
        {
            m_updated = true;
            m_kerning = s;
        }
    }

    Vector2f Text::GetDimensions() noexcept
    {
        Generate();
        return m_dimensions;
    }

    inline constexpr void NormalizeQuad(stbtt_aligned_quad& q, double s, double x, double y) noexcept
    {
        q.x1 = float((q.x1 * (s / 96.0)) + x);
        q.x0 = float((q.x0 * (s / 96.0)) + x);
        q.y0 = float((q.y0 * (s / 96.0)) + y);
        q.y1 = float((q.y1 * (s / 96.0)) + y);
    }

    void Text::Generate()
    {
        constexpr static float hOffset = 96 - 96.0 / 4.0;

        if(!s_defaultFont)
        {
            s_defaultFont = std::make_unique<Font>();
            s_defaultFont->LoadFromData(TML_DEFAULT_FONT_DATA);
        }

        m_dimensions = Vector2f{0, m_size.y};
        float x = 0, y = hOffset;
        float width = 0, height = 0;
        uint32_t count = 0;
        m_vertexData.clear();
        m_indexData.clear();
        const uint32_t hex = m_color.Hex();
        Font* font = m_font ? m_font.get() : s_defaultFont.get();

        for(auto c : m_string)
        {
            if(Math::InRange<int32_t>(c, 0, 8) || Math::InRange<int32_t>(c, 14, 31))
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
                    y += hOffset + m_lineSpacing;
                    x = 0;
                    width = 0;
                    break;

                case '\t':
                    x += m_size.x * 3 + m_kerning;
                    break;

                default:
                    stbtt_aligned_quad q{};

                    font->GetAlignedQuad(&q, (int)c - 32, x, y);
                    NormalizeQuad(q, m_size.x, m_pos.x, m_pos.y);

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