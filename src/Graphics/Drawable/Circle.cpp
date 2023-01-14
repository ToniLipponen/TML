#include <TML/Graphics/Drawable/Circle.h>
#include <TML/Graphics/RenderTarget.h>

namespace tml
{
    Circle::Circle()
    : Circle({0,0}, 0)
    {
        m_indexData = {0,1,2, 1,3,2};
    }

    Circle::Circle(const Vector2f &pos, float radius)
    {
        m_pos = pos;
        m_size = Vector2f{radius};
        m_color = Color(255, 255, 255);
        m_indexData = {0,1,2, 1,3,2};
    }

    void Circle::SetRadius(float r)
    {
        m_size = Vector2f{r};
        m_updated = true;
    }

    float Circle::GetRadius() const noexcept
    {
        return m_size.x;
    }

    void Circle::OnDraw(class RenderTarget* renderer, class Texture* circle) noexcept
    {
        if(m_updated)
        {
            const uint32_t hex = m_color.Hex();
            const auto s = m_size * m_scale;
            const auto cos_r = static_cast<float>(std::cos(Math::DegreesToRadians(m_rotation)));
            const auto sin_r = static_cast<float>(std::sin(Math::DegreesToRadians(m_rotation)));
            const auto offset = m_origin * -1;

            m_vertexData.clear();

            m_vertexData.push_back(Vertex{offset - s,                   {0.f, 1.f}, hex, Vertex::CIRCLE, 0});
            m_vertexData.push_back(Vertex{offset + Vector2f(s.x, -s.y), {1.f, 1.f}, hex, Vertex::CIRCLE, 0});
            m_vertexData.push_back(Vertex{offset + Vector2f(-s.x, s.y), {0.f, 0.f}, hex, Vertex::CIRCLE, 0});
            m_vertexData.push_back(Vertex{offset + s,                   {1.f, 0.f}, hex, Vertex::CIRCLE, 0});

            for(auto& v : m_vertexData)
            {
                v.pos = Math::Rotate(v.pos, cos_r, sin_r) + m_pos;
            }

            m_updated = false;
        }

        renderer->PushVertexData(m_vertexData, m_indexData, *circle);
    }
}