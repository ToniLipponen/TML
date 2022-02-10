#pragma once
#include <TML/Graphics/Vector2.h>
#include <cmath>

namespace tml
{
    class Transformable
    {
    public:
        inline Vector2f GetPosition()   const noexcept { return m_pos;      }
        inline Vector2f GetSize()       const noexcept { return m_size;     }
        inline float GetRotation()      const noexcept { return m_rotation; }

        virtual Vector2f Move(const Vector2f& offset) noexcept
        {
            return (m_pos += offset);
        }

        virtual Vector2f Move(float x, float y) noexcept
        {
            m_pos.x += x;
            m_pos.y += y;
            return m_pos;
        }

        virtual Vector2f SetPosition(const Vector2f& position) noexcept
        {
            return m_pos = position;
        }

        virtual Vector2f SetPosition(float x, float y) noexcept
        {
            return m_pos = {x, y};
        }

        virtual Vector2f Scale(const Vector2f& scale) noexcept
        {
            return m_size += scale;
        }

        virtual Vector2f Scale(float x, float y) noexcept
        {
            m_size.x += x;
            m_size.y += y;
            return m_size;
        }

        virtual Vector2f SetSize(const Vector2f& size) noexcept
        {
            return this->m_size = size;
        }

        virtual Vector2f SetSize(float x, float y) noexcept
        {
            return m_size = {x, y};
        }

        virtual float Rotate(float degrees) noexcept
        {
            m_rotation = std::fmod(m_rotation + degrees, 360.f);
            if (m_rotation < 0)
                m_rotation += 360.f;
            return m_rotation;
        }

        virtual float SetRotation(float degrees) noexcept
        {
            m_rotation = std::fmod(degrees, 360.f);
            if (m_rotation < 0)
                m_rotation += 360.f;
            return m_rotation;
        }

    protected:
        Vector2f m_pos = 0;
        Vector2f m_size = 0;
        float m_rotation = 0;
    };
}