#include <TML/Graphics/Drawable/Shape.h>
#include <TML/Graphics/RenderTarget.h>
#include <TML/System/Math.h>

#include <utility>

namespace tml
{
    Shape::Shape(std::vector<ShapePoint> points) noexcept
    : m_points(std::move(points))
    {
        m_updated = true;
    }

    Shape::Shape(std::vector<ShapePoint> points, const Texture& texture) noexcept
    : m_points(std::move(points)), m_texture(std::make_shared<Texture>())
    {
        *m_texture = texture;
    }

    Shape::Shape(std::vector<ShapePoint> points, std::shared_ptr<Texture> texture) noexcept
    : m_points(std::move(points)), m_texture(std::move(texture))
    {

    }

    uint32_t Shape::AddPoint(const ShapePoint& point) noexcept
    {
        m_points.push_back(point);
        m_updated = true;
        return m_points.size();
    }

    bool Shape::SetPoint(uint32_t index, const ShapePoint& point) noexcept
    {
        if(index < GetPointCount())
        {
            m_points[index] = point;
            m_updated = true;
            return true;
        }

        return false;
    }

    bool Shape::RemovePoint(uint32_t index) noexcept
    {
        if(index < GetPointCount())
        {
            m_points.erase(m_points.begin() + index);
            m_updated = true;
            return true;
        }

        return false;
    }

    bool Shape::ClearPoints() noexcept
    {
        if(GetPointCount())
        {
            m_points.clear();
            m_updated = true;
            return true;
        }

        return false;
    }

    Color Shape::SetColor(const Color &color) noexcept
    {
        m_color = color;

        for(auto& i : m_points)
        {
            i.color = color;
        }

		return m_color;
    }

    void Shape::SetTexture(const Texture& texture) noexcept
    {
        m_texture = std::make_shared<Texture>();
        *m_texture = texture;
    }

    void Shape::SetTexture(const std::shared_ptr<Texture>& texture) noexcept
    {
        m_texture = texture;
    }

    uint32_t Shape::GetPointCount() const noexcept
    {
        return m_points.size();
    }

    void Shape::OnDraw(RenderTarget* renderer, Texture*) noexcept
    {
        if(m_updated)
        {
            m_vertexData.clear();
            m_indexData.clear();

            const auto cos_r = static_cast<float>(std::cos(Math::DegreesToRadians(m_rotation)));
            const auto sin_r = static_cast<float>(std::sin(Math::DegreesToRadians(m_rotation)));
            const auto offset = m_origin * -1;

            for(auto& i : m_points)
            {
                m_vertexData.emplace_back(offset + i.pos, i.uv, i.color.Hex(), Vertex::COLOR, 0);
            }

            auto elements = static_cast<int64_t>(m_points.size());
            elements = Math::Max<int64_t>(elements-2, 0);

            for(int64_t i = 0; i < elements; ++i)
            {
                m_indexData.push_back(0);
                m_indexData.push_back(i+1);
                m_indexData.push_back(i+2);
            }

            for(auto& v : m_vertexData)
            {
                v.pos = Math::Rotate(v.pos * m_scale, cos_r, sin_r) + m_pos;
            }

            m_updated = false;
        }

        if(m_texture)
        {
            renderer->PushVertexData(m_vertexData, m_indexData, *m_texture);
        }
        else
        {
            renderer->PushVertexData(m_vertexData, m_indexData);
        }
    }
}