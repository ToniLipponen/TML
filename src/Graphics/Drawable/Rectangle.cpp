#include <TML/Graphics/Drawable/Rectangle.h>
#include <TML/Utilities/Math.h>

namespace tml
{
    Rectangle::Rectangle()
    {
        m_pos = {0,0};
        m_size = {0,0};
        Generate();
    }

    Rectangle::Rectangle(float x, float y, float w, float h)
    {
        m_pos = {x,y};
        m_size = {w,h};
        Generate();
    }

    Rectangle::Rectangle(const Vector2f& position, const Vector2f& size)
    {
        m_pos = position;
        m_size = size;
        Generate();
    }

    void Rectangle::Generate() noexcept
    {
        const Vector2f origin = m_pos + (m_size * 0.5);
        const ui32 col = m_color.Hex();
        m_vertexData.clear();
        m_indexData.clear();

        m_vertexData.emplace_back(Vertex{Math::Rotate(origin, m_pos, m_rotation),                          {0.f, 0.f}, col, 0, Vertex::COLOR});
        m_vertexData.emplace_back(Vertex{Math::Rotate(origin, m_pos + Vector2f(m_size.x, 0.f), m_rotation),{1.f, 0.f}, col, 0, Vertex::COLOR});
        m_vertexData.emplace_back(Vertex{Math::Rotate(origin, m_pos + Vector2f(0.f, m_size.y), m_rotation),{0.f, 1.f}, col, 0, Vertex::COLOR});
        m_vertexData.emplace_back(Vertex{Math::Rotate(origin, m_pos + m_size, m_rotation),                 {1.f, 1.f}, col, 0, Vertex::COLOR});

        m_indexData.push_back(0);
        m_indexData.push_back(1);
        m_indexData.push_back(2);

        m_indexData.push_back(1);
        m_indexData.push_back(3);
        m_indexData.push_back(2);
    }
}