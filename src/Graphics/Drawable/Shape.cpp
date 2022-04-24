#include <TML/Graphics/Drawable/Shape.h>
#include <TML/Graphics/Renderer.h>
#include <TML/System/Math.h>


namespace tml
{
    uint32_t Shape::AddPoint(const Vector2f& position) noexcept
    {
        m_points.push_back({position, m_color});
        m_updated = true;
        return m_points.size();
    }

    uint32_t Shape::AddPoint(const Vector2f& position, const Color& color) noexcept
    {
        m_points.push_back({position, color});
        m_updated = true;
        return m_points.size();
    }

    uint32_t Shape::AddPoint(const ShapePoint& point) noexcept
    {
        m_points.push_back(point);
        m_updated = true;
        return m_points.size();
    }

    bool Shape::SetPoint(uint32_t index, const Vector2f& position) noexcept
    {
        if(index < GetPointCount())
        {
            m_points[index] = {position, m_color};
            m_updated = true;
            return true;
        }
        return false;
    }

    bool Shape::SetPoint(uint32_t index, const Vector2f& position, const Color& color) noexcept
    {
        if(index < GetPointCount())
        {
            m_points[index] = {position, color};
            m_updated = true;
            return true;
        }
        return false;
    }

    bool Shape::SetPoint(uint32_t index, const ShapePoint& point)
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

    uint32_t Shape::GetPointCount() const noexcept
    {
        return m_points.size();
    }

    void Shape::OnDraw(class Renderer* renderer, class Texture *) noexcept
    {
        if(m_updated)
        {
            m_vertexData.clear();
            m_indexData.clear();

            if(m_rotation == 0)
            {
                for(auto& i : m_points)
                {
                    m_vertexData.push_back(Vertex{i.pos+m_pos, {0, 0}, i.color.Hex(), Vertex::COLOR});
                }
            }
            else
            {
                const float cos_r = std::cos(Math::DegToRad(m_rotation));
                const float sin_r = std::sin(Math::DegToRad(m_rotation));

                for(auto& i : m_points)
                {
                    m_vertexData.push_back(Vertex{Math::Rotate(m_pos + m_origin, i.pos+m_pos, cos_r, sin_r), {0, 0}, i.color.Hex(), Vertex::COLOR});
                }
            }

            auto elements = static_cast<int64_t>(m_points.size());
            elements = Math::Max<int64_t>(elements-2, 0);

            for(int64_t i = 0; i < elements; ++i)
            {
                m_indexData.push_back(0);
                m_indexData.push_back(i+1);
                m_indexData.push_back(i+2);
            }
            m_updated = false;
        }
        renderer->PushVertexData(m_vertexData, m_indexData);
    }
}