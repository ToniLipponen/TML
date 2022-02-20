#include <TML/Graphics/Drawable/Rectangle.h>
#include "TML/System/Math.h"

namespace tml
{
    Rectangle::Rectangle()
    {
        m_pos = {0,0};
        m_size = {0,0};
        Rectangle::Generate();
    }

    Rectangle::Rectangle(float x, float y, float w, float h)
    {
        m_pos = {x,y};
        m_size = {w,h};
        Rectangle::Generate();
    }

    Rectangle::Rectangle(const Vector2f& position, const Vector2f& size)
    {
        m_pos = position;
        m_size = size;
        Rectangle::Generate();
    }

    void Rectangle::Generate() noexcept
    {
        const ui32 col = m_color.Hex();
        m_vertexData.clear();
        m_indexData.clear();

        if(m_rotation != 0)
        {
            const float cos_r = std::cos(Math::DegToRad(m_rotation));
            const float sin_r = std::sin(Math::DegToRad(m_rotation));
            m_vertexData.emplace_back(Vertex{Math::Rotate(m_pos + m_origin, m_pos, cos_r, sin_r),                          {0.f, 0.f}, col, Vertex::COLOR});
            m_vertexData.emplace_back(Vertex{Math::Rotate(m_pos + m_origin, m_pos + Vector2f(m_size.x, 0.f), cos_r, sin_r),{1.f, 0.f}, col, Vertex::COLOR});
            m_vertexData.emplace_back(Vertex{Math::Rotate(m_pos + m_origin, m_pos + Vector2f(0.f, m_size.y), cos_r, sin_r),{0.f, 1.f}, col, Vertex::COLOR});
            m_vertexData.emplace_back(Vertex{Math::Rotate(m_pos + m_origin, m_pos + m_size, cos_r, sin_r),                 {1.f, 1.f}, col, Vertex::COLOR});
        }
        else
        {
            m_vertexData.emplace_back(Vertex{m_pos,                          {0.f, 0.f}, col, Vertex::COLOR});
            m_vertexData.emplace_back(Vertex{m_pos + Vector2f(m_size.x, 0.f),{1.f, 0.f}, col, Vertex::COLOR});
            m_vertexData.emplace_back(Vertex{m_pos + Vector2f(0.f, m_size.y),{0.f, 1.f}, col, Vertex::COLOR});
            m_vertexData.emplace_back(Vertex{m_pos + m_size,                 {1.f, 1.f}, col, Vertex::COLOR});
        }

        m_indexData.push_back(0);
        m_indexData.push_back(1);
        m_indexData.push_back(2);

        m_indexData.push_back(1);
        m_indexData.push_back(3);
        m_indexData.push_back(2);
    }
}