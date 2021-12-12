#include <TML/Graphics/Drawable/Rectangle.h>
#include <TML/Utilities/Math.h>

namespace tml
{
    Rectangle::Rectangle()
    {
        Rectangle({0, 0}, {100, 100});
    }

    Rectangle::Rectangle(float x, float y, float w, float h)
    {
        Rectangle({x,y}, {w,h});
    }

    Rectangle::Rectangle(const Vector2f& position, const Vector2f& size)
    {
        m_pos = position;
        m_size = size;
        m_indexData = std::vector<ui32>(6,0);
        m_vertexData = std::vector<Vertex>(4);
        Generate();
    }

    void Rectangle::Generate() noexcept
    {
        const Vector2f origin = (m_pos + m_pos + m_size) * 0.5f;
        const ui32 col = m_color.Hex();

        m_vertexData.at(0) = {Math::Rotate(origin, m_pos, m_rotation),                          {0.f, 0.f}, col, 0, Vertex::COLOR};
        m_vertexData.at(1) = {Math::Rotate(origin, m_pos + Vector2f(m_size.x, 0.f), m_rotation),{1.f, 0.f}, col, 0, Vertex::COLOR};
        m_vertexData.at(2) = {Math::Rotate(origin, m_pos + Vector2f(0.f, m_size.y), m_rotation),{0.f, 1.f}, col, 0, Vertex::COLOR};
        m_vertexData.at(3) = {Math::Rotate(origin, m_pos + m_size, m_rotation),                 {1.f, 1.f}, col, 0, Vertex::COLOR};

        m_indexData.at(0) = 0;
        m_indexData.at(1) = 1;
        m_indexData.at(2) = 2;

        m_indexData.at(3) = 1;
        m_indexData.at(4) = 3;
        m_indexData.at(5) = 2;
    }
}