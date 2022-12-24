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
        m_size = {1,1};
    }

    Line::Line(const Vector2f& a, const Vector2f& b, float thickness, const Color& color)
    : a(a), b(b)
    {
        m_size = {thickness};
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

            const auto cos_r = static_cast<float>(std::cos(Math::DegreesToRadians(m_rotation)));
            const auto sin_r = static_cast<float>(std::sin(Math::DegreesToRadians(m_rotation)));

            const auto hex = m_color.Hex();

            const float dx = b.x - a.x;
            const float dy = b.y - a.y;
            const auto normalA = (Vector2f(-dy, dx).Normalized() * m_size.x / 2);
            const auto normalB = (Vector2f(dy, -dx).Normalized() * m_size.x / 2);
            const auto dir = Math::Normalize(b - a) * (m_size.x / 2);
            const auto offset = m_origin * -1;

            m_vertexData.push_back(Vertex{(offset + normalA + a), {0, 0}, hex, Vertex::COLOR, 0});
            m_vertexData.push_back(Vertex{(offset + normalB + a), {0, 0}, hex, Vertex::COLOR, 0});
            m_vertexData.push_back(Vertex{(offset + normalA + b), {0, 0}, hex, Vertex::COLOR, 0});
            m_vertexData.push_back(Vertex{(offset + normalB + b), {0, 0}, hex, Vertex::COLOR, 0});

            if(m_rounded)
            {
                m_vertexData.emplace_back(Vertex{offset + normalA + a - dir, {0.0, 1.0}, hex, Vertex::TEXT, 0});
                m_vertexData.emplace_back(Vertex{offset + normalB + a - dir, {1.0, 1.0}, hex, Vertex::TEXT, 0});
                m_vertexData.emplace_back(Vertex{offset + normalA + a,       {0.0, 0.5}, hex, Vertex::TEXT, 0});
                m_vertexData.emplace_back(Vertex{offset + normalB + a,       {1.0, 0.5}, hex, Vertex::TEXT, 0});

                m_vertexData.emplace_back(Vertex{offset + normalA + b,       {0.0, 0.5}, hex, Vertex::TEXT, 0});
                m_vertexData.emplace_back(Vertex{offset + normalB + b,       {1.0, 0.5}, hex, Vertex::TEXT, 0});
                m_vertexData.emplace_back(Vertex{offset + normalA + b + dir, {0.0, 1.0}, hex, Vertex::TEXT, 0});
                m_vertexData.emplace_back(Vertex{offset + normalB + b + dir, {1.0, 1.0}, hex, Vertex::TEXT, 0});
                m_indexData.insert(m_indexData.end(), {4,5,6, 5,7,6, 8,9,10, 9,11,10});
            }

            for(auto& v : m_vertexData)
            {
                v.pos = Math::Rotate(v.pos * m_scale, cos_r, sin_r) + m_pos;
            }

            m_updated = false;
        }

        renderer->PushVertexData(m_vertexData, m_indexData, *circle);
    }
}
