#include <TML/Graphics/Drawable/Circle.h>

namespace tml
{
    Circle::Circle()
    : Circle({0,0}, 0)
    {

    }

    Circle::Circle(const Vector2f &pos, float radius)
    {
        radius = Math::Max(radius, 1.0f);
        m_pos = pos;
        m_size = Vector2f{radius, radius};
        m_color = Color(255, 255, 255);
        Circle::Generate();
    }

    void Circle::SetRadius(float r)
    {
        r = Math::Max(r, 0.1f);
        m_size = Vector2f{r, r};
        Circle::Generate();
    }

    float Circle::GetRadius() const noexcept
    {
        return m_size.x;
    }

    void Circle::Generate() noexcept
    {
        const ui32 hex = m_color.Hex();
        const Vector2f topLeft = m_pos - m_size.x;
        const float x2 = m_size.x * 2;
        m_vertexData.clear();
        m_indexData.clear();

        if(m_rotation != 0)
        {
            const float cos_r = std::cos(Math::DegToRad(m_rotation));
            const float sin_r = std::sin(Math::DegToRad(m_rotation));
            m_vertexData.push_back(Vertex{Math::Rotate(m_pos + m_origin, topLeft, cos_r, sin_r),                    {0.f, 0.f}, hex, Vertex::TEXT});
            m_vertexData.push_back(Vertex{Math::Rotate(m_pos + m_origin, topLeft + Vector2f(x2, 0.f), cos_r, sin_r),{1.f, 0.f}, hex, Vertex::TEXT});
            m_vertexData.push_back(Vertex{Math::Rotate(m_pos + m_origin, topLeft + Vector2f(0.f, x2), cos_r, sin_r),{0.f, 1.f}, hex, Vertex::TEXT});
            m_vertexData.push_back(Vertex{Math::Rotate(m_pos + m_origin, topLeft + x2, cos_r, sin_r),               {1.f, 1.f}, hex, Vertex::TEXT});
        }
        else
        {
            m_vertexData.push_back(Vertex{topLeft,                    {0.f, 0.f}, hex, Vertex::TEXT});
            m_vertexData.push_back(Vertex{topLeft + Vector2f(x2, 0.f),{1.f, 0.f}, hex, Vertex::TEXT});
            m_vertexData.push_back(Vertex{topLeft + Vector2f(0.f, x2),{0.f, 1.f}, hex, Vertex::TEXT});
            m_vertexData.push_back(Vertex{topLeft + x2,               {1.f, 1.f}, hex, Vertex::TEXT});
        }

        m_indexData.push_back(0);
        m_indexData.push_back(1);
        m_indexData.push_back(2);

        m_indexData.push_back(1);
        m_indexData.push_back(3);
        m_indexData.push_back(2);
    }

}