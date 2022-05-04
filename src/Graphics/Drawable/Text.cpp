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

#define TAB_SIZE 4

namespace tml
{
    Text::Text()
    : m_leading(0), m_kerning(0), m_dimensions(0), m_font(nullptr), m_string("")
    {
        m_color = Color{255,255,255,255};
        m_pos   = Vector2f{0,0};
        m_size  = Vector2f{32,32};
    }

    Text::Text(String string, const Vector2f& pos, float size, const Color& color)
    : m_leading(0), m_kerning(0), m_dimensions(0), m_font(nullptr), m_string(std::move(string))
    {
        m_color = color;
        m_pos   = pos;
        m_size  = Vector2f{size, size};
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

    void Text::SetLeading(float leading) noexcept
    {
        if(m_leading != leading)
        {
            m_updated = true;
            m_leading = leading;
        }
    }

    void Text::SetTracking(float tracking) noexcept
    {
        if(m_tracking != tracking)
        {
            m_updated = true;
            m_tracking = tracking;
        }
    }

    void Text::SetKerning(float kerning) noexcept
    {
        if(m_kerning != kerning)
        {
            m_updated = true;
            m_kerning = kerning;
        }
    }

    const String& Text::GetString() const noexcept
    {
        return m_string;
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
        m_dimensions = Vector2f{0, m_size.y};

        float x    = 0;
        float y    = 96.0f - 96.0f / 4.0f;
        float ypos = 0;
        float xpos = 0;

        m_vertexData.clear();
        m_indexData.clear();

        uint32_t count = 0;
        const uint32_t hex = m_color.Hex();
        Font& font = m_font ? *m_font : GetDefaultFont();

        char32_t previousChar = 0;

        for(auto c : m_string)
        {
            if(Math::InRange<int32_t>((int)c, 0, 8) || Math::InRange<int32_t>((int)c, 14, 31))
                continue;

            switch(c)
            {
                case 11:
                case 12:
                    break;
                case '\r':
                case '\n':
                {
                    m_dimensions.y += m_size.x + m_leading;
                    ypos += m_size.x + m_leading;
                    xpos = 0;
                    x = 0;
                } break;

                case '\t':
                    x += m_size.x * TAB_SIZE + m_tracking;
                    break;

                default:
                {
                    stbtt_aligned_quad q{};

                    const float kerning = m_kerning * font.GetKerning((int)previousChar, (int)c) * (m_size.y / 96);
                    font.GetAlignedQuad(&q, (int)c - 32, x, y);
                    NormalizeQuad(q, m_size.x, m_pos.x, m_pos.y);

                    q.x0 += kerning + xpos;
                    q.x1 += kerning + xpos;
                    q.y0 += ypos;
                    q.y1 += ypos;

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
                    xpos += m_tracking;
                    m_dimensions.x = Math::Max(q.x1 - m_pos.x, m_dimensions.x);
                } break;
            }

            previousChar = c;
        }
    }

    void Text::OnDraw(class Renderer* renderer, Texture*) noexcept
    {
        if(m_updated)
        {
            Generate();
            m_updated = false;
        }
        renderer->PushVertexData(m_vertexData, m_indexData, m_font ? m_font->m_texture : GetDefaultFont().m_texture);
    }

    Font& Text::GetDefaultFont() noexcept
    {
        static Font* defaultFont = new Font;
        static bool defaultFontInitialized = false;

        if(!defaultFontInitialized)
        {
            defaultFont->LoadFromData(TML_DEFAULT_FONT_DATA);
            defaultFontInitialized = true;
        }

        return *defaultFont;
    }
}