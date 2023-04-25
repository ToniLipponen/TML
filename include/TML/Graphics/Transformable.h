#pragma once
#include <TML/System/Math/Vector2.h>
#include <TML/System/Math.h>
#include <TML/Export.h>

namespace tml
{
    class TML_API Transformable
    {
    public:
        [[nodiscard]] Vector2f GetPosition() const noexcept 
        { 
            return m_pos;
        }

        [[nodiscard]] Vector2f GetSize() const noexcept 
        {
            return m_size;
        }

        [[nodiscard]] Vector2f GetOrigin() const noexcept 
        {
            return m_origin;
        }

        [[nodiscard]] Vector2f GetScale() const noexcept 
        {
            return m_scale;
        }

        [[nodiscard]] float GetRotation() const noexcept 
        {
            return m_rotation;
        }

        Vector2f Move(const Vector2f& offset) noexcept
        {
            m_updated = true;
            return (m_pos += offset);
        }

        Vector2f Move(float x, float y) noexcept
        {
            m_pos.x += x;
            m_pos.y += y;
            m_updated = true;
            return m_pos;
        }
        
        Vector2f SetPosition(const Vector2f& position) noexcept
        {
            m_updated = true;
            return m_pos = position;
        }

        Vector2f SetPosition(float x, float y) noexcept
        {
            m_updated = true;
            return m_pos = {x, y};
        }

        Vector2f SetSize(const Vector2f& size) noexcept
        {
            m_updated = true;
            return this->m_size = size;
        }

        Vector2f SetSize(float x, float y) noexcept
        {
            m_updated = true;
            return m_size = {x, y};
        }

        float Rotate(float degrees) noexcept
        {
            m_rotation = std::fmod(m_rotation + degrees, 360.f);

            if(m_rotation < 0)
            {
                m_rotation += 360.f;
            }

            m_updated = true;
            return m_rotation;
        }

        float SetRotation(float degrees) noexcept
        {
            m_rotation = std::fmod(degrees, 360.f);

            if(m_rotation < 0)
            {
                m_rotation += 360.f;
            }

            m_updated = true;
            return m_rotation;
        }

        Vector2f SetOrigin(const Vector2f& origin) noexcept
        {
            m_origin = origin;
            m_updated = true;
            return m_origin;
        }

        Vector2f SetOrigin(float x, float y) noexcept
        {
            m_origin = Vector2f(x,y);
            m_updated = true;
            return m_origin;
        }

        Vector2f Scale(const Vector2f& multiplier) noexcept
        {
            m_scale = m_scale * multiplier;
            m_updated = true;
            return m_scale;
        }

        Vector2f Scale(float x, float y) noexcept
        {
            m_scale = m_scale * Vector2f(x, y);
            m_updated = true;
            return m_scale;
        }

        Vector2f SetScale(const Vector2f& scale) noexcept
        {
            m_scale = scale;
            m_updated = true;
            return m_scale;
        }

        Vector2f SetScale(float x, float y) noexcept
        {
            m_scale = Vector2f(x,y);
            m_updated = true;
            return m_scale;
        }

    protected:
        Vector2f m_pos = {0,0};
        Vector2f m_size = {0,0};
        Vector2f m_origin = {0,0};
        Vector2f m_scale = {1,1};
        float m_rotation = 0;
        bool m_updated = true;
    };
}