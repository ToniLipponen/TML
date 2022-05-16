#include <TML/Graphics/Drawable/Line.h>
#include <TML/Graphics/RenderTarget.h>
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

    void Line::SetThickness(float thickness) noexcept
    {
        m_size = {thickness};
        m_updated = true;
    }

    void Line::SetRounded(bool rounded) noexcept
    {
        if(m_rounded != rounded)
        {
            m_updated = true;
            m_rounded = rounded;
        }
    }

    void Line::OnDraw(RenderTarget* renderer, Texture* circle) noexcept
    {
        if(m_updated)
        {
            m_vertexData.clear();
            m_indexData = {0,1,2, 1,3,2};

            Vector2f rotatedA = a, rotatedB = b;

            const auto cos_r = static_cast<float>(std::cos(Math::DegToRad(m_rotation)));
            const auto sin_r = static_cast<float>(std::sin(Math::DegToRad(m_rotation)));

            if(m_rotation != 0)
            {
                rotatedA = Math::Rotate(m_origin, a, cos_r, sin_r);
                rotatedB = Math::Rotate(m_origin, b, cos_r, sin_r);
            }

            if(m_applyOriginToPosition)
            {
                rotatedA -= m_origin;
                rotatedB -= m_origin;
            }

            rotatedA = rotatedA + m_pos;
            rotatedB = rotatedB + m_pos;
            const auto hex = m_color.Hex();

            const float dx = rotatedB.x - rotatedA.x;
            const float dy = rotatedB.y - rotatedA.y;
            const auto normalA = (Vector2f(-dy, dx).Normalized()  *  m_size.x / 2);
            const auto normalB = (Vector2f(dy, -dx).Normalized()  *  m_size.x / 2);
            const auto dir = Math::Normalize(rotatedB - rotatedA) * (m_size.x / 2);

            m_vertexData.emplace_back(Vertex{(normalA + rotatedA), {0, 0}, hex, Vertex::COLOR});
            m_vertexData.emplace_back(Vertex{(normalB + rotatedA), {0, 0}, hex, Vertex::COLOR});
            m_vertexData.emplace_back(Vertex{(normalA + rotatedB), {0, 0}, hex, Vertex::COLOR});
            m_vertexData.emplace_back(Vertex{(normalB + rotatedB), {0, 0}, hex, Vertex::COLOR});

            if(m_rounded)
            {
                m_vertexData.emplace_back(Vertex{normalA + rotatedA - dir, {0.0, 1.0}, hex, Vertex::TEXT});
                m_vertexData.emplace_back(Vertex{normalB + rotatedA - dir, {1.0, 1.0}, hex, Vertex::TEXT});
                m_vertexData.emplace_back(Vertex{normalA + rotatedA,       {0.0, 0.5}, hex, Vertex::TEXT});
                m_vertexData.emplace_back(Vertex{normalB + rotatedA,       {1.0, 0.5}, hex, Vertex::TEXT});

                m_vertexData.emplace_back(Vertex{normalA + rotatedB,       {0.0, 0.5}, hex, Vertex::TEXT});
                m_vertexData.emplace_back(Vertex{normalB + rotatedB,       {1.0, 0.5}, hex, Vertex::TEXT});
                m_vertexData.emplace_back(Vertex{normalA + rotatedB + dir, {0.0, 1.0}, hex, Vertex::TEXT});
                m_vertexData.emplace_back(Vertex{normalB + rotatedB + dir, {1.0, 1.0}, hex, Vertex::TEXT});
                m_indexData.insert(m_indexData.end(), {4,5,6, 5,7,6, 8,9,10, 9,11,10});
            }

            m_updated = false;
        }
        renderer->PushVertexData(m_vertexData, m_indexData, *circle);
    }
}
