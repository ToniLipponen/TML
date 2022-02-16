#pragma once
#include <TML/Graphics/Drawable/Shape.h>
#include <TML/System/Math.h>

namespace tml
{
    ui32 Shape::AddPoint(const Vector2f& position) noexcept
    {
        m_points.push_back({position, m_color});
        Generate();
        return m_points.size();
    }

    ui32 Shape::AddPoint(const Vector2f& position, const Color& color) noexcept
    {
        m_points.push_back({position, color});
        Generate();
        return m_points.size();
    }

    ui32 Shape::AddPoint(const ShapePoint& point) noexcept
    {
        m_points.push_back(point);
        Generate();
        return m_points.size();
    }

    bool Shape::SetPoint(ui32 index, const Vector2f& position) noexcept
    {
        if(index < GetPointCount())
        {
            m_points[index] = {position, m_color};
            Generate();
            return true;
        }
        return false;
    }

    bool Shape::SetPoint(ui32 index, const Vector2f& position, const Color& color) noexcept
    {
        if(index < GetPointCount())
        {
            m_points[index] = {position, color};
            Generate();
            return true;
        }
        return false;
    }

    bool Shape::SetPoint(ui32 index, const ShapePoint& point)
    {
        if(index < GetPointCount())
        {
            m_points[index] = point;
            Generate();
            return true;
        }
        return false;
    }

    bool Shape::RemovePoint(ui32 index) noexcept
    {
        if(index < GetPointCount())
        {
            m_points.erase(m_points.begin() + index);
            Generate();
            return true;
        }
        return false;
    }

    bool Shape::ClearPoints() noexcept
    {
        if(GetPointCount())
        {
            m_points.clear();
            Generate();
            return true;
        }
        return false;
    }

    ui32 Shape::GetPointCount() const noexcept
    {
        return m_points.size();
    }

    void Shape::Generate() noexcept
    {
        m_vertexData.clear();
        m_indexData.clear();

        if(m_rotation == 0)
        {
            for(auto& i : m_points)
            {
                m_vertexData.push_back(Vertex{i.pos+m_pos, {0, 0}, i.color.Hex(), 0, Vertex::COLOR});
            }
        }
        else
        {
            Vector2f sum = m_pos * m_points.size();
            for(auto& i : m_points)
            {
                sum = sum + i.pos;
            }
            const Vector2f origin = sum / m_points.size();

            for(auto& i : m_points)
            {
                m_vertexData.push_back(Vertex{Math::Rotate(origin, i.pos+m_pos, m_rotation), {0, 0}, i.color.Hex(), 0, Vertex::COLOR});
            }
        }

        i64 elements = static_cast<i64>(m_points.size());
        elements = Math::Max<i64>(elements-2, 0);

        for(i64 i = 0; i < elements; ++i)
        {
            m_indexData.push_back(0);
            m_indexData.push_back(i+1);
            m_indexData.push_back(i+2);
        }
    }
}