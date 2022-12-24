#include <TML/Graphics/Drawable/Rectangle.h>
#include <TML/Graphics/RenderTarget.h>
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

    void Rectangle::OnDraw(RenderTarget* renderer, Texture* circle) noexcept
    {
        if(m_updated)
        {
            const auto clampedRoundness = Math::Clamp<float>(m_roundness, 0, Math::Min<float>(m_size.y, m_size.x) / 2);
            const auto hex = m_color.Hex();
            const auto w = Vector2f{m_size.x, 0.f};
            const auto h = Vector2f{0.f, m_size.y};
            const auto rx = Vector2f{clampedRoundness, 0.f};
            const auto ry = Vector2f{0.f, clampedRoundness};

            const auto cos_r = static_cast<float>(std::cos(Math::DegreesToRadians(m_rotation)));
            const auto sin_r = static_cast<float>(std::sin(Math::DegreesToRadians(m_rotation)));

            const auto offset = m_origin * -1;

            m_vertexData.clear();
            m_indexData.clear();

            if(m_roundness < 0.001)
            {
                m_vertexData.push_back(Vertex{offset, {0.f, 0.f}, hex, Vertex::COLOR, 0});
                m_vertexData.push_back(Vertex{offset + Vector2f(m_size.x, 0.f), {1.f, 0.f}, hex, Vertex::COLOR, 0});
                m_vertexData.push_back(Vertex{offset + Vector2f(0.f, m_size.y), {0.f, 1.f}, hex, Vertex::COLOR, 0});
                m_vertexData.push_back(Vertex{offset + m_size, {1.f, 1.f}, hex, Vertex::COLOR, 0});
                m_indexData = {0,1,2, 1,3,2};
            }
            else
            {
                m_indexData =
                {
                     0, 1, 2,    1, 3, 2,
                     4, 5, 6,    5, 7, 6,
                     8, 9,10,    9,11,10,
                    12,13,14,   13,15,14,
                    16,17,18,   16,19,18,
                    20,21,22,   20,23,22,
                    24,25,26,   25,27,26,
                    28,29,30,   29,31,30
                };

                m_vertexData.push_back(Vertex{offset,            {0.0f, 0.0f}, hex, Vertex::TEXT, 0});
                m_vertexData.push_back(Vertex{offset + rx,       {0.5f, 0.0f}, hex, Vertex::TEXT, 0});
                m_vertexData.push_back(Vertex{offset + ry,       {0.0f, 0.5f}, hex, Vertex::TEXT, 0});
                m_vertexData.push_back(Vertex{offset + rx + ry,  {0.5f, 0.5f}, hex, Vertex::TEXT, 0});

                m_vertexData.push_back(Vertex{offset + w - rx,       {0.5f, 0.0f}, hex, Vertex::TEXT, 0});
                m_vertexData.push_back(Vertex{offset + w,            {1.0f, 0.0f}, hex, Vertex::TEXT, 0});
                m_vertexData.push_back(Vertex{offset + w - rx + ry,  {0.5f, 0.5f}, hex, Vertex::TEXT, 0});
                m_vertexData.push_back(Vertex{offset + w + ry,       {1.0f, 0.5f}, hex, Vertex::TEXT, 0});

                m_vertexData.push_back(Vertex{offset + w + h - rx - ry,  {0.5f, 0.5f}, hex, Vertex::TEXT, 0});
                m_vertexData.push_back(Vertex{offset + w + h - ry,       {1.0f, 0.5f}, hex, Vertex::TEXT, 0});
                m_vertexData.push_back(Vertex{offset + w + h - rx,       {0.5f, 1.0f}, hex, Vertex::TEXT, 0});
                m_vertexData.push_back(Vertex{offset + w + h,            {1.0f, 1.0f}, hex, Vertex::TEXT, 0});

                m_vertexData.push_back(Vertex{offset + h - ry,       {0.0f, 0.5f}, hex, Vertex::TEXT, 0});
                m_vertexData.push_back(Vertex{offset + h + rx - ry,  {0.5f, 0.5f}, hex, Vertex::TEXT, 0});
                m_vertexData.push_back(Vertex{offset + h,            {0.0f, 1.0f}, hex, Vertex::TEXT, 0});
                m_vertexData.push_back(Vertex{offset + h + rx,       {0.5f, 1.0f}, hex, Vertex::TEXT, 0});

                // top rect
                const auto pos2 = rx;
                const auto size = w+ry-rx-rx;
                m_vertexData.push_back(Vertex{offset + pos2,                        {0.0f, 0.5f}, hex, Vertex::COLOR, 0});
                m_vertexData.push_back(Vertex{offset + pos2 + Vector2f(size.x, 0),  {0.5f, 0.5f}, hex, Vertex::COLOR, 0});
                m_vertexData.push_back(Vertex{offset + pos2 + size,                 {0.5f, 1.0f}, hex, Vertex::COLOR, 0});
                m_vertexData.push_back(Vertex{offset + pos2 + Vector2f(0, size.y),  {0.0f, 1.0f}, hex, Vertex::COLOR, 0});

                // bottom rect
                const auto pos3 = Vector2f(0,m_size.y) + rx - ry;
                m_vertexData.push_back(Vertex{offset + pos3,                        {0.0f, 0.5f}, hex, Vertex::COLOR, 0});
                m_vertexData.push_back(Vertex{offset + pos3 + Vector2f(size.x, 0),  {0.5f, 0.5f}, hex, Vertex::COLOR, 0});
                m_vertexData.push_back(Vertex{offset + pos3 + size,                 {0.5f, 1.0f}, hex, Vertex::COLOR, 0});
                m_vertexData.push_back(Vertex{offset + pos3 + Vector2f(0, size.y),  {0.0f, 1.0f}, hex, Vertex::COLOR, 0});

                m_vertexData.push_back(Vertex{offset + ry,                          {0,0}, hex, Vertex::COLOR, 0});
                m_vertexData.push_back(Vertex{offset + ry + Vector2f{m_size.x, 0.f},{0,0}, hex, Vertex::COLOR, 0});
                m_vertexData.push_back(Vertex{offset - ry + Vector2f{0.f, m_size.y},{0,0}, hex, Vertex::COLOR, 0});
                m_vertexData.push_back(Vertex{offset - ry + m_size,                 {0,0}, hex, Vertex::COLOR, 0});
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