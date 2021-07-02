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

void Text::Generate()
{
    float x = 0, y = 128;
    float d = 1.f / 2048;
    
    int count = 0;
    for(auto c : m_string)
    {
        Font::FontChar fc = m_font.m_chars.at(c);
        if(c == ' ')
        {
            x += 3;
            continue;
        }
        Vector2 s = fc.size / 128.f;
        float t = fc.top / 128.f;
        s *= 32;
        t *= 32;
        // m_vertexData.push_back(Vertex{m_pos + Vector2{x,t},                      m_color,fc.pos * d,                          1, 0.f});
        // m_vertexData.push_back(Vertex{m_pos + Vector2{x,t} + Vector2{s.x, 0.f},  m_color,(fc.pos + Vector2{fc.size.x,0}) * d, 1, 0.f});
        // m_vertexData.push_back(Vertex{m_pos + Vector2{x,t} + Vector2{0, s.y},    m_color,(fc.pos + Vector2{0,fc.size.y}) * d, 1, 0.f});
        // m_vertexData.push_back(Vertex{m_pos + Vector2{x,t} + Vector2{s.x, s.y},  m_color,(fc.pos + fc.size) * d,              1, 0.f});

        m_vertexData.push_back(Vertex{m_pos + Vector2{x,y-fc.top},                                  m_color,fc.pos * d,                          1, 0.f});
        m_vertexData.push_back(Vertex{m_pos + Vector2{x,y-fc.top} + Vector2{fc.size.x, 0.f},        m_color,(fc.pos + Vector2{fc.size.x,0}) * d, 1, 0.f});
        m_vertexData.push_back(Vertex{m_pos + Vector2{x,y-fc.top} + Vector2{0, fc.size.y},          m_color,(fc.pos + Vector2{0,fc.size.y}) * d, 1, 0.f});
        m_vertexData.push_back(Vertex{m_pos + Vector2{x,y-fc.top} + Vector2{fc.size.x, fc.size.y},  m_color,(fc.pos + fc.size) * d,              1, 0.f});

        m_indexData.push_back(count + 0);
        m_indexData.push_back(count + 1);
        m_indexData.push_back(count + 2);

        m_indexData.push_back(count + 1);
        m_indexData.push_back(count + 3);
        m_indexData.push_back(count + 2);
        
        count += 4;
        x += fc.size.x;
    }
}