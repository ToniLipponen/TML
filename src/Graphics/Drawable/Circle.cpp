#include <TML/Graphics/Drawable/Circle.h>
#include <TML/Graphics/Renderer.h>

namespace tml
{
    Circle::Circle()
    : Circle({0,0}, 0)
    {
        m_indexData = {0,1,2, 1,3,2};
    }

    Circle::Circle(const Vector2f &pos, float radius)
    {
        radius = Math::Max(radius, 1.0f);
        m_pos = pos;
        m_size = Vector2f{radius, radius};
        m_color = Color(255, 255, 255);
        m_indexData = {0,1,2, 1,3,2};
    }

    void Circle::SetRadius(float r)
    {
        r = Math::Max(r, 0.1f);
        m_size = Vector2f{r, r};
        m_updated = true;
    }

    float Circle::GetRadius() const noexcept
    {
        return m_size.x;
    }

    void Circle::OnDraw(class Renderer *renderer, class Texture* circle) noexcept
    {
        if(m_updated)
        {
            const ui32 hex = m_color.Hex();
            const Vector2f topLeft = m_pos - m_size.x;
            const float x2 = m_size.x * 2;
            m_vertexData.clear();
            if(m_rotation != 0)
            {
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
            m_updated = false;
        }
        renderer->PushVertexData(m_vertexData, m_indexData, *circle);
    }
}