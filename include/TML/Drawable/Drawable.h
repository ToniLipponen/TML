#pragma once
#include "../Types.h"
#include "../Texture.h"

namespace tml
{
    class Drawable
    {
    public:
        virtual void SetColor(const Color &c) noexcept
        {
            m_color = c;
        }

        virtual void SetPosition(const Vector2 &p) noexcept
        {
            m_pos = p;
        }

        virtual void SetPosition(float x, float y) noexcept
        {
            m_pos = {x, y};
        }

        virtual void SetSize(const Vector2 &s) noexcept
        {
            m_size = s;
        }

        void SetTexture(const Texture &t) noexcept
        {
            m_tex = t;
        }

        void Rotate(float d) noexcept
        {
            m_rotation += d;
        }

        void SetRotation(float d) noexcept {
            m_rotation = d;
        }

        void Translate(const Vector2 &offset) noexcept
        {
            m_pos += offset;
        }

        void Translate(float x, float y) noexcept
        {
            m_pos += {x,y};
        }

        const Vector2& GetSize() const noexcept
        {
            return m_size;
        }

        const Vector2& GetPosition() const noexcept
        {
            return m_pos;
        }

        constexpr float GetRotation() const noexcept
        {
            return m_rotation;
        }

        constexpr const Color& GetColor() const noexcept
        {
            return m_color;
        }

        friend class Renderer;

    protected:
        Color m_color = WHITE;
        Vector2 m_pos = {0, 0};
        Vector2 m_size = {0, 0};
        Texture m_tex;
        float m_rotation = 0;
    };
}