#include <TML/Drawable/Text.h>
#include <stb/stb_truetype.h>
#include "internal/Default_font.h"

using namespace tml;

Text::Text()
{
    m_color = {255,255,255};
    m_pos = {0,0};
    m_size = {32,32};
    m_font.LoadFromMemory(DEFAULT_FONT_DATA.data(), DEFAULT_FONT_DATA.size());
}

Text::Text(std::string text)
: m_string(std::move(text))
{
    m_color = {255,255,255};
    m_pos = {0,0};
    m_size = {32,32};
    m_font.LoadFromMemory(DEFAULT_FONT_DATA.data(), DEFAULT_FONT_DATA.size());
    Generate();
}

Text::Text(std::string text, const std::string& font_file_name)
: m_string(std::move(text))
{
    m_font.LoadFromFile(font_file_name);
    m_color = {255,255,255};
    m_pos = {0,0};
    m_size = {32,32};
    Generate();
}

Text::Text(std::string text, Font& font)
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

[[maybe_unused]] void Text::SetString(std::string string)
{
    m_string = std::move(string);
    Generate();
}

void Text::SetFont(const Font &font)
{
    m_font = font;
    Generate();
}

[[maybe_unused]] void Text::SetSpacing(ui32 s)
{
    m_spacing = s;
    Generate();
}

constexpr void NormalizeQuad(stbtt_aligned_quad& q, float x) noexcept
{
    q.x0 *= (x / 256.0);
    q.x1 *= (x / 256.0);
    q.y0 *= (x / 256.0);
    q.y1 *= (x / 256.0);
}

void Text::Generate() noexcept
{
    float x = m_pos.x, y = m_pos.y + 256.f;
    int count = 0;
    m_vertexData.clear();
    m_indexData.clear();
    stbtt_aligned_quad q;
    for(auto c : m_string)
    {
        stbtt_GetBakedQuad((stbtt_bakedchar*)m_font.m_cdata, 2048, 2048,int(c-32), &x, &y,&q, 1);
        NormalizeQuad(q, m_size.x);
        m_vertexData.push_back({{q.x0, q.y0}, m_color.Hex(), {q.s0, q.t0}, 0, Vertex::TEXT});
        m_vertexData.push_back({{q.x1, q.y0}, m_color.Hex(), {q.s1, q.t0}, 0, Vertex::TEXT});
        m_vertexData.push_back({{q.x0, q.y1}, m_color.Hex(), {q.s0, q.t1}, 0, Vertex::TEXT});
        m_vertexData.push_back({{q.x1, q.y1}, m_color.Hex(), {q.s1, q.t1}, 0, Vertex::TEXT});

        m_indexData.push_back(count + 0);
        m_indexData.push_back(count + 1);
        m_indexData.push_back(count + 2);

        m_indexData.push_back(count + 1);
        m_indexData.push_back(count + 3);
        m_indexData.push_back(count + 2);
        
        count += 4;
    }
    m_width     = x;
}