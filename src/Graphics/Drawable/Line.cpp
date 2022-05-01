#include <TML/Graphics/Drawable/Line.h>
#include "../Renderer.h"
#include <TML/System/Math.h>

namespace tml
{
    Line::Line()
    : a(0,0), b(0,0)
    {
        m_size.x = 0;
        m_updated = true;
        m_indexData = {0,1,2, 1,3,2};
    }

    Line::Line(const Vector2f& a, const Vector2f& b, float thickness, const Color& color)
    : a(a), b(b)
    {
        m_size.x = thickness;
        m_color = color;
        m_updated = true;
        m_indexData = {0,1,2, 1,3,2};
    }

    void Line::SetPointA(const Vector2f& point) noexcept
    {
        a = point;
        m_updated = true;
    }

    void Line::SetPointB(const Vector2f& point) noexcept
    {
        b = point;
        m_updated = true;
    }

    void Line::OnDraw(Renderer* renderer, Texture *) noexcept
    {
        if(m_updated)
        {
            m_vertexData.clear();
            Vector2f rotatedA = a, rotatedB = b;
            if(m_rotation != 0)
            {
                rotatedA = Math::Rotate(m_origin, a, cos_r, sin_r);
                rotatedB = Math::Rotate(m_origin, b, cos_r, sin_r);
            }
            rotatedA = rotatedA + m_pos;
            rotatedB = rotatedB + m_pos;
            const float dx = rotatedB.x - rotatedA.x;
            const float dy = rotatedB.y - rotatedA.y;
            const auto dirA = (Vector2f(-dy, dx).Normalized() * m_size.x * 0.5);
            const auto dirB = (Vector2f(dy, -dx).Normalized() * m_size.x * 0.5);

            m_vertexData.emplace_back(Vertex{(dirA + rotatedA), {0, 0}, m_color.Hex(), Vertex::COLOR});
            m_vertexData.emplace_back(Vertex{(dirB + rotatedA), {0, 0}, m_color.Hex(), Vertex::COLOR});
            m_vertexData.emplace_back(Vertex{(dirA + rotatedB), {0, 0}, m_color.Hex(), Vertex::COLOR});
            m_vertexData.emplace_back(Vertex{(dirB + rotatedB), {0, 0}, m_color.Hex(), Vertex::COLOR});
            m_updated = false;
        }
        renderer->PushVertexData(m_vertexData, m_indexData);
    }
}
