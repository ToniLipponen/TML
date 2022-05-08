#include <TML/Graphics/Drawable/Rectangle.h>
#include "../Renderer.h"
#include <TML/System/Math.h>

namespace tml
{
    Rectangle::Rectangle()
    {
        m_pos = {0,0};
        m_size = {0,0};
    }

    Rectangle::Rectangle(float x, float y, float w, float h)
    {
        m_pos = {x,y};
        m_size = {w,h};
    }

    Rectangle::Rectangle(const Vector2f& position, const Vector2f& size)
    {
        m_pos = position;
        m_size = size;
    }

    void Rectangle::SetCornerRadius(float r) noexcept
    {
        m_roundness = r;
        m_updated = true;
    }

    void Rectangle::OnDraw(Renderer *renderer, Texture* circle) noexcept
    {
        if(m_updated)
        {
            const auto hex = m_color.Hex();
            const auto w = Vector2f{m_size.x, 0.f};
            const auto h = Vector2f{0.f, m_size.y};
            const auto rx = Vector2f{m_roundness, 0.f};
            const auto ry = Vector2f{0.f, m_roundness};
            m_vertexData.clear();
            m_indexData.clear();

            if(m_roundness < 2)
            {
                if(m_rotation != 0)
                {
                    m_vertexData.emplace_back(Vertex{Math::Rotate(m_pos + m_origin, m_pos, cos_r, sin_r),                          {0.f, 0.f}, hex, Vertex::COLOR});
                    m_vertexData.emplace_back(Vertex{Math::Rotate(m_pos + m_origin, m_pos + Vector2f(m_size.x, 0.f), cos_r, sin_r),{1.f, 0.f}, hex, Vertex::COLOR});
                    m_vertexData.emplace_back(Vertex{Math::Rotate(m_pos + m_origin, m_pos + Vector2f(0.f, m_size.y), cos_r, sin_r),{0.f, 1.f}, hex, Vertex::COLOR});
                    m_vertexData.emplace_back(Vertex{Math::Rotate(m_pos + m_origin, m_pos + m_size, cos_r, sin_r),                 {1.f, 1.f}, hex, Vertex::COLOR});
                }
                else
                {
                    m_vertexData.emplace_back(Vertex{m_pos,                          {0.f, 0.f}, hex, Vertex::COLOR});
                    m_vertexData.emplace_back(Vertex{m_pos + Vector2f(m_size.x, 0.f),{1.f, 0.f}, hex, Vertex::COLOR});
                    m_vertexData.emplace_back(Vertex{m_pos + Vector2f(0.f, m_size.y),{0.f, 1.f}, hex, Vertex::COLOR});
                    m_vertexData.emplace_back(Vertex{m_pos + m_size,                 {1.f, 1.f}, hex, Vertex::COLOR});
                }

                m_indexData = {0,1,2, 1,3,2};
            }
            else
            {
                m_indexData = {
                        0, 1, 2,    1, 3, 2,
                        4, 5, 6,    5, 7, 6,
                        8, 9, 10,   9, 11,10,
                        12,13,14,   13,15,14,
                        16,17,18,   16,19,18,
                        20,21,22,   20,23,22,
                        24,25,26,   25,27,26,
                        28,29,30,   29,31,30
                };

                const auto origin = m_origin + m_pos;
                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos, cos_r, sin_r), {0.0f, 0.0f}, hex, Vertex::TEXT});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + rx, cos_r, sin_r), {0.5f, 0.0f}, hex, Vertex::TEXT});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + ry, cos_r, sin_r), {0.0f, 0.5f}, hex, Vertex::TEXT});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + rx + ry, cos_r, sin_r), {0.5f, 0.5f}, hex, Vertex::TEXT});

                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + w - rx, cos_r, sin_r), {0.5f, 0.0f}, hex, Vertex::TEXT});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + w, cos_r, sin_r), {1.0f, 0.0f}, hex, Vertex::TEXT});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + w - rx + ry, cos_r, sin_r), {0.5f, 0.5f}, hex, Vertex::TEXT});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + w + ry, cos_r, sin_r), {1.0f, 0.5f}, hex, Vertex::TEXT});

                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + w + h - rx - ry, cos_r, sin_r), {0.5f, 0.5f}, hex, Vertex::TEXT});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + w + h - ry, cos_r, sin_r), {1.0f, 0.5f}, hex, Vertex::TEXT});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + w + h - rx, cos_r, sin_r), {0.5f, 1.0f}, hex, Vertex::TEXT});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + w + h, cos_r, sin_r), {1.0f, 1.0f}, hex, Vertex::TEXT});

                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + h - ry, cos_r, sin_r), {0.0f, 0.5f}, hex, Vertex::TEXT});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + h + rx - ry, cos_r, sin_r), {0.5f, 0.5f}, hex, Vertex::TEXT});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + h, cos_r, sin_r), {0.0f, 1.0f}, hex, Vertex::TEXT});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + h + rx, cos_r, sin_r), {0.5f, 1.0f}, hex, Vertex::TEXT});

                // top rect
                const auto pos2 = m_pos + rx;
                const auto size = w+ry-rx-rx;
                m_vertexData.push_back(Vertex{Math::Rotate(origin, pos2, cos_r, sin_r), {0.0f, 0.5f}, hex, Vertex::COLOR});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, pos2 + Vector2f(size.x, 0), cos_r, sin_r), {0.5f, 0.5f}, hex, Vertex::COLOR});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, pos2 + size, cos_r, sin_r), {0.5f, 1.0f}, hex, Vertex::COLOR});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, pos2 + Vector2f(0, size.y), cos_r, sin_r), {0.0f, 1.0f}, hex, Vertex::COLOR});

                // bottom rect
                const auto pos3 = m_pos + Vector2f(0,m_size.y) + rx - ry;
                m_vertexData.push_back(Vertex{Math::Rotate(origin, pos3, cos_r, sin_r), {0.0f, 0.5f}, hex, Vertex::COLOR});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, pos3 + Vector2f(size.x, 0), cos_r, sin_r), {0.5f, 0.5f}, hex, Vertex::COLOR});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, pos3 + size, cos_r, sin_r), {0.5f, 1.0f}, hex, Vertex::COLOR});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, pos3 + Vector2f(0, size.y), cos_r, sin_r), {0.0f, 1.0f}, hex, Vertex::COLOR});

                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + ry, cos_r, sin_r),                          {0,0}, hex, Vertex::COLOR});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos + ry + Vector2f{m_size.x, 0.f}, cos_r, sin_r),{0,0}, hex, Vertex::COLOR});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos - ry + Vector2f{0.f, m_size.y}, cos_r, sin_r),{0,0}, hex, Vertex::COLOR});
                m_vertexData.push_back(Vertex{Math::Rotate(origin, m_pos - ry + m_size, cos_r, sin_r),                 {0,0}, hex, Vertex::COLOR});
            }
            m_updated = false;
        }
        renderer->PushVertexData(m_vertexData, m_indexData, *circle);
    }
}