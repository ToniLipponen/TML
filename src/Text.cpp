#include "../include/Drawable/Text.h"



Text::Text()
{
    m_color = {255,255,255};
    m_pos = {0,0};
    m_size = {32,32};
}

Text::Text(const std::string& text, const std::string& font_file_name)
{
    m_string = text;
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

void Text::SetSize(const Vector2& size)
{
    m_size = size;
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

void Text::Generate()
{
    float x = 0, y = 256;
    constexpr static float d = 1.f / 2048;
    constexpr static float cs = 1.f / 256.f;
    float dm = cs * m_size.x;
    int count = 0;
    m_vertexData.clear();
    m_indexData.clear();
    for(auto c : m_string)
    {
        Font::FontChar fc = m_font.m_chars.at(c);
        if(c == ' ')
        {
            x += m_size.x;
            continue;
        }

        m_vertexData.push_back(Vertex{(Vector2{x,y-fc.top} * dm) + m_pos,                                    m_color,fc.pos * d,                          1, 0.f, TEXT});
        m_vertexData.push_back(Vertex{((Vector2{x,y-fc.top} + Vector2{fc.size.x, 0.f}) * dm) + m_pos,        m_color,(fc.pos + Vector2{fc.size.x,0}) * d, 1, 0.f, TEXT});
        m_vertexData.push_back(Vertex{((Vector2{x,y-fc.top} + Vector2{0, fc.size.y}) * dm) + m_pos,          m_color,(fc.pos + Vector2{0,fc.size.y}) * d, 1, 0.f, TEXT});
        m_vertexData.push_back(Vertex{((Vector2{x,y-fc.top} + Vector2{fc.size.x, fc.size.y}) * dm) + m_pos,  m_color,(fc.pos + fc.size) * d,              1, 0.f, TEXT});

        m_indexData.push_back(count + 0);
        m_indexData.push_back(count + 1);
        m_indexData.push_back(count + 2);

        m_indexData.push_back(count + 1);
        m_indexData.push_back(count + 3);
        m_indexData.push_back(count + 2);
        
        count += 4;
        x += fc.size.x;
        // x += s.x;
    }
}