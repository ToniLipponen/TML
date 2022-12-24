#pragma once
#include <TML/System/Math/Vector2.h>
#include <TML/System/Math.h>
#include <TML/Export.h>

namespace tml
{
    class TML_API Transformable
    {
    public:
        virtual ~Transformable() noexcept = default;
        [[nodiscard]] constexpr inline Vector2f GetPosition()   const noexcept { return m_pos;      }
        [[nodiscard]] constexpr inline Vector2f GetSize()       const noexcept { return m_size;     }
        [[nodiscard]] constexpr inline Vector2f GetOrigin()     const noexcept { return m_origin;   }
        [[nodiscard]] constexpr inline Vector2f GetScale()      const noexcept { return m_scale;    }
        [[nodiscard]] constexpr inline float GetRotation()      const noexcept { return m_rotation; }

        constexpr inline Vector2f Move(const Vector2f& offset) noexcept
        {
            m_updated = true;
            return (m_pos += offset);
        }

        constexpr inline Vector2f Move(float x, float y) noexcept
        {
            m_pos.x += x;
            m_pos.y += y;
            m_updated = true;
            return m_pos;
        }

        constexpr inline Vector2f SetPosition(const Vector2f& position) noexcept
        {
            m_updated = true;
            return m_pos = position;
        }

        constexpr inline Vector2f SetPosition(float x, float y) noexcept
        {
            m_updated = true;
            return m_pos = {x, y};
        }

        constexpr inline Vector2f SetSize(const Vector2f& size) noexcept
        {
            m_updated = true;
            return this->m_size = size;
        }

        constexpr inline Vector2f SetSize(float x, float y) noexcept
        {
            m_updated = true;
            return m_size = {x, y};
        }

        inline float Rotate(float degrees) noexcept
        {
            m_rotation = std::fmod(m_rotation + degrees, 360.f);

            if(m_rotation < 0)
            {
                m_rotation += 360.f;
            }

            m_updated = true;
            return m_rotation;
        }

        inline float SetRotation(float degrees) noexcept
        {
            m_rotation = std::fmod(degrees, 360.f);

            if(m_rotation < 0)
            {
                m_rotation += 360.f;
            }

            m_updated = true;
            return m_rotation;
        }

        constexpr inline Vector2f SetOrigin(const Vector2f& origin) noexcept
        {
            m_origin = origin;
            m_updated = true;
            return m_origin;
        }

        constexpr inline Vector2f SetOrigin(float x, float y) noexcept
        {
            m_origin = Vector2f(x,y);
            m_updated = true;
            return m_origin;
        }

        constexpr inline Vector2f Scale(const Vector2f& multiplier) noexcept
        {
            m_scale = m_scale * multiplier;
            m_updated = true;
            return m_scale;
        }

        constexpr inline Vector2f Scale(float x, float y) noexcept
        {
            m_scale = m_scale * Vector2f(x, y);
            m_updated = true;
            return m_scale;
        }

        inline Vector2f SetScale(const Vector2f& scale) noexcept
        {
            m_scale = scale;
            m_updated = true;
            return m_scale;
        }

        inline Vector2f SetScale(float x, float y) noexcept
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