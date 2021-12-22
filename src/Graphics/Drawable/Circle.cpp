#include <TML/Graphics/Drawable/Circle.h>

namespace tml
{
    Circle::Circle()
    {
        Circle({0}, 0);
    }

    Circle::Circle(const Vector2f &pos, float radius)
    {
        radius = Math::Max(radius, 1.0f);
        m_pos = pos;
        m_size = Vector2f{radius, radius};
        m_color = Color(255, 255, 255);

        m_indexData = std::vector<ui32>(6, 0);
        m_vertexData = std::vector<Vertex>(4);
        Generate();
    }

    void Circle::SetRadius(float r)
    {
        r = Math::Min(r, 0.1f);
        m_size = Vector2f{r, r};
        Generate();
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
//        auto* data = &m_vertexData[0];

        m_vertexData[0] = {topLeft,                    {0.f, 0.f}, hex, 0, Vertex::TEXT};
        m_vertexData[1] = {topLeft + Vector2f(x2, 0.f),{1.f, 0.f}, hex, 0, Vertex::TEXT};
        m_vertexData[2] = {topLeft + Vector2f(0.f, x2),{0.f, 1.f}, hex, 0, Vertex::TEXT};
        m_vertexData[3] = {topLeft + x2,               {1.f, 1.f}, hex, 0, Vertex::TEXT};

        m_indexData.at(0) = 0;
        m_indexData.at(1) = 1;
        m_indexData.at(2) = 2;

        m_indexData.at(3) = 1;
        m_indexData.at(4) = 3;
        m_indexData.at(5) = 2;
    }
}