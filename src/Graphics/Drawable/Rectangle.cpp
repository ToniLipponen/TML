#include <TML/Graphics/Drawable/Rectangle.h>
#include <TML/Utilities/Math.h>

namespace tml
{
    Rectangle::Rectangle()
    {
        m_pos = Vector2f{0.f, 0.f};
        m_size = Vector2f{100, 100};
        m_indexData.reserve(8);
        m_vertexData.reserve(8);
    }

    Rectangle::Rectangle(float x, float y, float w, float h)
    {
        m_pos = Vector2f{x, y};
        m_size = Vector2f{w, h};
    }

    Rectangle::Rectangle(const Vector2f& position, const Vector2f& size)
    {
        m_pos = position;
        m_size = size;
    }

    void Rectangle::Generate() noexcept
    {
        const Vector2f origin = (m_pos + m_pos + m_size) * 0.5f;
        const ui32 col = m_color.Hex();
        auto* data = m_vertexData.data();

        data[0] = {Math::Rotate(origin, m_pos, m_rotation),                          {0.f, 0.f}, col, 0, Vertex::COLOR};
        data[1] = {Math::Rotate(origin, m_pos + Vector2f(m_size.x, 0.f), m_rotation),{1.f, 0.f}, col, 0, Vertex::COLOR};
        data[2] = {Math::Rotate(origin, m_pos + Vector2f(0.f, m_size.y), m_rotation),{0.f, 1.f}, col, 0, Vertex::COLOR};
        data[3] = {Math::Rotate(origin, m_pos + m_size, m_rotation),                 {1.f, 1.f}, col, 0, Vertex::COLOR};

        m_indexData.at(0) = 0;
        m_indexData.at(1) = 1;
        m_indexData.at(2) = 2;

        m_indexData.at(3) = 1;
        m_indexData.at(4) = 3;
        m_indexData.at(5) = 2;
    }
}