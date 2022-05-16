#include <TML/Graphics/Drawable/Circle.h>
#include <TML/Graphics/RenderTarget.h>

namespace tml
{
    Circle::Circle()
    : Circle({0,0}, 0)
    {
        m_indexData = {0,1,2, 1,3,2};
        m_applyOriginToPosition = true;
    }

    Circle::Circle(const Vector2f &pos, float radius)
    {
        m_pos = pos;
        m_size = Vector2f{radius*2};
        m_origin = Vector2f{radius};
        m_color = Color(255, 255, 255);
        m_indexData = {0,1,2, 1,3,2};
        m_applyOriginToPosition = true;
    }

    void Circle::SetRadius(float r)
    {
        if(m_size.x != r)
        {
            m_size = Vector2f{r * 2};
            m_origin = Vector2f{r};
            m_updated = true;
        }
    }

    float Circle::GetRadius() const noexcept
    {
        return m_size.x / 2;
    }

    void Circle::OnDraw(class RenderTarget* renderer, class Texture* circle) noexcept
    {
        if(m_updated)
        {
            const uint32_t hex = m_color.Hex();
            const float r = m_size.x;
            const auto cos_r = static_cast<float>(std::cos(Math::DegToRad(m_rotation)));
            const auto sin_r = static_cast<float>(std::sin(Math::DegToRad(m_rotation)));
            const auto offset = m_applyOriginToPosition ? m_origin * -1 : Vector2f();

            m_vertexData.clear();

            if(m_rotation != 0)
            {
                m_vertexData.emplace_back(Vertex{offset + Math::Rotate(m_pos + m_origin, m_pos, cos_r, sin_r),                    {0.f, 0.f}, hex, Vertex::TEXT});
                m_vertexData.emplace_back(Vertex{offset + Math::Rotate(m_pos + m_origin, m_pos + Vector2f(r, 0.f), cos_r, sin_r), {1.f, 0.f}, hex, Vertex::TEXT});
                m_vertexData.emplace_back(Vertex{offset + Math::Rotate(m_pos + m_origin, m_pos + Vector2f(0.f, r), cos_r, sin_r), {0.f, 1.f}, hex, Vertex::TEXT});
                m_vertexData.emplace_back(Vertex{offset + Math::Rotate(m_pos + m_origin, m_pos + r, cos_r, sin_r), {1.f, 1.f}, hex, Vertex::TEXT});
            }
            else
            {
                m_vertexData.emplace_back(Vertex{offset + m_pos,                    {0.f, 1.f}, hex, Vertex::TEXT});
                m_vertexData.emplace_back(Vertex{offset + m_pos + Vector2f(r, 0.f), {1.f, 1.f}, hex, Vertex::TEXT});
                m_vertexData.emplace_back(Vertex{offset + m_pos + Vector2f(0.f, r), {0.f, 0.f}, hex, Vertex::TEXT});
                m_vertexData.emplace_back(Vertex{offset + m_pos + r,                {1.f, 0.f}, hex, Vertex::TEXT});
            }

            m_updated = false;
        }
        renderer->PushVertexData(m_vertexData, m_indexData, *circle);
    }
}