#include <TML/Drawable/Text.h>
#include <TML/Utilities/Utilities.h>
#include <stb/stb_truetype.h>
#include "internal/Default_font.h"

using namespace tml;
Text* DEFAULT_TEXT;

Text::Text()
{
    m_color = {255,255,255};
    m_pos = {0,0};
    m_size = {32,32};
    m_font.LoadFromMemory(DEFAULT_FONT_DATA.data(), DEFAULT_FONT_DATA.size());
}

Text::Text(std::wstring text)
: m_string(std::move(text))
{
    m_color = {255,255,255};
    m_pos = {0,0};
    m_size = {32,32};
    m_font.LoadFromMemory(DEFAULT_FONT_DATA.data(), DEFAULT_FONT_DATA.size());
    Generate();
}

Text::Text(std::wstring text, const std::string& font_file_name)
: m_string(std::move(text))
{
    m_font.LoadFromFile(font_file_name);
    m_color = {255,255,255};
    m_pos = {0,0};
    m_size = {32,32};
    Generate();
}

Text::Text(std::wstring text, Font& font)
: m_string(std::move(text))
{
    m_font = font;
    m_color = {255,255,255};
    m_pos = {0,0};
    m_size = {32,32};
    Generate();
}

void Text::SetPosition(const Vector2 &pos) noexcept
{
    if(m_pos.x == pos.x && m_pos.y == pos.y) // Checking this every time is faster than Generate()
        return;
    m_pos = pos;
    Generate();
}

void Text::SetSize(float s)
{
    m_size = {s,s};
    Generate();
}

void Text::SetColor(const Color& color) noexcept
{
    m_color = color;
    Generate();
}

void Text::SetString(std::wstring string)
{
    m_string = std::move(string);
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
    (q.y0 = (q.y0 * (s / 64.0))) += y - (s / 3.f);
    (q.y1 = (q.y1 * (s / 64.0))) += y - (s / 3.f);
}

void Text::Generate() noexcept
{
    m_dimensions = {0, m_size.y};
    float x = 0, y = 64.f;
    float width = 0, height = 0;
    ui32 count = 0;
    m_vertexData.clear();
    m_indexData.clear();
    for(auto c : m_string)
    {
        stbtt_aligned_quad q;
        if(c == '\n')
        {
            m_dimensions.x = Util::Min(width, m_dimensions.x);
            m_dimensions.y = Util::Min(height, m_dimensions.y);
            y += 64.f;
            x = 0;
            width = 0;
            continue;
        }
        stbtt_GetBakedQuad((stbtt_bakedchar*)m_font.m_cdata, 1024, 1024,int(c-32), &x, &y,&q, 1);
        NormalizeQuad(q, m_size.x, m_pos.x, m_pos.y);
        m_vertexData.push_back({{q.x0, q.y0}, {q.s0, q.t0}, m_color.Hex(), 0, Vertex::TEXT});
        m_vertexData.push_back({{q.x1, q.y0}, {q.s1, q.t0}, m_color.Hex(), 0, Vertex::TEXT});
        m_vertexData.push_back({{q.x0, q.y1}, {q.s0, q.t1}, m_color.Hex(), 0, Vertex::TEXT});
        m_vertexData.push_back({{q.x1, q.y1}, {q.s1, q.t1}, m_color.Hex(), 0, Vertex::TEXT});

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
    m_dimensions.x = Util::Min(width, m_dimensions.x);
    m_dimensions.y = Util::Min(height, m_dimensions.y);
}