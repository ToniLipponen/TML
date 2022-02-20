#include <TML/Graphics/Drawable/Line.h>
#include <TML/System/Math.h>

namespace tml
{
    Line::Line()
    : a(0,0), b(0,0)
    {
        m_size.x = 0;
        Line::Generate();
    }

    Line::Line(const Vector2f& a, const Vector2f& b, ui32 thickness, const Color& color)
    : a(a), b(b)
    {
        m_size.x = thickness;
        m_color = color;
        Line::Generate();
    }

    void Line::SetPointA(const Vector2f& point) noexcept
    {
        a = point;
        Generate();
    }

    void Line::SetPointB(const Vector2f& point) noexcept
    {
        b = point;
        Generate();
    }

    void Line::Generate() noexcept
    {
        m_vertexData.clear();
        m_indexData.clear();
        Vector2f rotatedA = a, rotatedB = b;
        if(m_rotation != 0)
        {
            const auto cos_r = static_cast<float>(std::cos(Math::DegToRad(m_rotation)));
            const auto sin_r = static_cast<float>(std::sin(Math::DegToRad(m_rotation)));
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

        m_indexData.push_back(0);
        m_indexData.push_back(1);
        m_indexData.push_back(2);

        m_indexData.push_back(1);
        m_indexData.push_back(3);
        m_indexData.push_back(2);
    }
}
