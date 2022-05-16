#pragma once
#include <TML/System/Math/Vector2.h>
#include <TML/System/Math.h>
#include <TML/Export.h>

namespace tml
{
    class TML_API Transformable
    {
    public:
        [[nodiscard]] constexpr inline Vector2f GetPosition()   const noexcept { return m_pos;      }
        [[nodiscard]] constexpr inline Vector2f GetSize()       const noexcept { return m_size;     }
        [[nodiscard]] constexpr inline Vector2f GetOrigin()     const noexcept { return m_origin;   }
        [[nodiscard]] constexpr inline float GetRotation()      const noexcept { return m_rotation; }
        [[nodiscard]] virtual Vector2f GetCenter() noexcept
        {
            if(m_applyOriginToPosition)
                return (m_pos + m_size / 2) - m_origin;

            return (m_pos + m_size / 2);
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
            if (m_rotation < 0)
                m_rotation += 360.f;
            m_updated = true;
            return m_rotation;
        }

        float SetRotation(float degrees) noexcept
        {
            m_rotation = std::fmod(degrees, 360.f);

            if(m_rotation < 0)
                m_rotation += 360.f;

            m_updated = true;
            return m_rotation;
        }

        void SetOrigin(const Vector2f& origin) noexcept
        {
            m_origin = origin;
            m_updated = true;
        }

        void ApplyOriginToPosition(bool value) noexcept
        {
            m_applyOriginToPosition = value;
        }

    protected:
        Vector2f m_pos = 0;
        Vector2f m_size = 0;
        Vector2f m_origin = 0;
        float m_rotation = 0;
        bool m_updated = true;
        bool m_applyOriginToPosition = true;
    };
}