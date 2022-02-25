#pragma once
#include <TML/System/Math/Vector2.h>
#include <TML/System/Math.h>

namespace tml
{
    class Transformable
    {
    public:
        inline Vector2f GetPosition()   const noexcept { return m_pos;      }
        inline Vector2f GetSize()       const noexcept { return m_size;     }
        inline float GetRotation()      const noexcept { return m_rotation; }

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
            cos_r = std::cos(Math::DegToRad(m_rotation));
            sin_r = std::sin(Math::DegToRad(m_rotation));
            if (m_rotation < 0)
                m_rotation += 360.f;
            m_updated = true;
            return m_rotation;
        }

        float SetRotation(float degrees) noexcept
        {
            m_rotation = std::fmod(degrees, 360.f);
            cos_r = std::cos(Math::DegToRad(m_rotation));
            sin_r = std::sin(Math::DegToRad(m_rotation));
            if (m_rotation < 0)
                m_rotation += 360.f;
            m_updated = true;
            return m_rotation;
        }

        void SetOrigin(const Vector2f& origin) noexcept
        {
            m_origin = origin;
            m_updated = true;
        }

    protected:
        Vector2f m_pos = 0;
        Vector2f m_size = 0;
        Vector2f m_origin = {0, 0};
        float m_rotation = 0;
        float cos_r = 1, sin_r = 0;
        bool m_updated = true;
    };
}