#include "../include/Drawable/Text.h"
#include "../include/Default_font.h"
#include "../include/Utilities/Utilities.h"

using namespace tml;

Text::Text()
{
    m_color = {255,255,255};
    m_pos = {0,0};
    m_size = {32,32};
    m_font.LoadFromMemory(DEFAULT_FONT_DATA.data(), DEFAULT_FONT_DATA.size());
}

Text::Text(const std::string& text)
: m_string(text)
{
    m_color = {255,255,255};
    m_pos = {0,0};
    m_size = {32,32};
    m_font.LoadFromMemory(DEFAULT_FONT_DATA.data(), DEFAULT_FONT_DATA.size());
    Generate();
}

Text::Text(const std::string& text, const std::string& font_file_name)
: m_string(text)
{
    m_font.LoadFromFile(font_file_name);
    m_color = {255,255,255};
    m_pos = {0,0};
    m_size = {32,32};
    Generate();
}

Text::Text(const std::string& text, Font& font)
{
    m_string = text;
    m_font = font;
    m_color = {255,255,255};
    m_pos = {0,0};
    m_size = {32,32};
    Generate();
}

void Text::SetPosition(const Vector2 &pos)
{
    m_pos = pos;
    Generate();
}

void Text::SetSize(float s)
{
    m_size = {s,s};
    Generate();
}

void Text::SetColor(const Color& color)
{
    m_color = color;
    Generate();
}

void Text::SetString(const std::string &string)
{
    m_string = string;
    Generate();
}

void Text::SetFont(const Font &font)
{
    m_font = font;
    Generate();
}

void Text::SetSpacing(ui32 s)
{
    m_spacing = s;
    Generate();
}

#define GetCharSize(ax, bx, as) (bx - ax) * as / 385.f * m_size.x
#define GetSpacing(size) (size / 10) * m_spacing

void Text::Generate()
{
    if(m_font.m_chars.empty()) // Font failed to load? No glyphs available.
        return;
    float x = 0, y = 32;
    int count = 0;
    m_vertexData.clear();
    m_indexData.clear();
    const Color col = m_color * 0.003921568f;
    for(auto c : m_string)
    {
        if(c == '\n')
        {
            x = 0;
            y += m_size.x;
            m_height += 385.f * m_size.x / (4096 / 3);
            continue;
        }
        const Font::FontChar& fc = m_font.m_chars.at(c);
        const float cw = GetCharSize(fc.pos.x, fc.size.x, 4096);
        const float ch = GetCharSize(fc.pos.y, fc.size.y, 4096);
        // This is a mess, please fix
        m_vertexData.push_back(Vertex{(Vector2{x-fabsf(fc.offset.x*385),y-fabsf(fc.offset.y*385)}) + m_pos,
            col,fc.pos,1, 0.f, Vertex::TEXT});
        m_vertexData.push_back(Vertex{((Vector2{x-fabsf(fc.offset.x*385),y-fabsf(fc.offset.y*385)} + Vector2{cw, 0.f})) + m_pos,
            col,Vector2{fc.size.x,fc.pos.y},1, 0.f, Vertex::TEXT});
        m_vertexData.push_back(Vertex{((Vector2{x-fabsf(fc.offset.x*385),y-fabsf(fc.offset.y*385)} + Vector2{0, ch})) + m_pos,
            col,Vector2{fc.pos.x,fc.size.y},1, 0.f, Vertex::TEXT});
        m_vertexData.push_back(Vertex{((Vector2{x-fabsf(fc.offset.x*385),y-fabsf(fc.offset.y*385)} + Vector2{cw, ch})) + m_pos,
            col,fc.size,1, 0.f, Vertex::TEXT});
        m_indexData.push_back(count + 0);
        m_indexData.push_back(count + 1);
        m_indexData.push_back(count + 2);

        m_indexData.push_back(count + 1);
        m_indexData.push_back(count + 3);
        m_indexData.push_back(count + 2);
        m_height = Util::Min(ch, m_height);
        
        count += 4;
        x += GetCharSize(fc.pos.x, fc.size.x, 4096) + GetSpacing(m_size.x);
    }
    m_width     = x;
}