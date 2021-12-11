#pragma once
#include <TML/Utilities/Types.h>
#include <TML/Graphics/Texture.h>
#include <TML/Graphics/Drawable/Vertex.h>
#include <vector>

namespace tml
{
    class Drawable
    {
    public:
        virtual void SetColor(const Color &c) noexcept
        {
            m_color = c;
            Generate();
        }

        virtual void SetPosition(const Vector2f &p) noexcept
        {
            if(m_pos.x == p.x && m_pos.y == p.y) return;
            m_pos = p;
            Generate();
        }

        virtual void SetPosition(float x, float y) noexcept
        {
            m_pos = {x, y};
            Generate();
        }

        virtual void SetSize(const Vector2f &s) noexcept
        {
            m_size = s;
            Generate();
        }

        void SetTexture(const Texture &t) noexcept
        {
            m_tex = t;
            Generate();
        }

        void Rotate(float d) noexcept
        {
            m_rotation += d;
            Generate();
        }

        void SetRotation(float d) noexcept
        {
            m_rotation = d;
            Generate();
        }

        void Translate(const Vector2f &offset) noexcept
        {
            m_pos += offset;
            Generate();
        }

        void Translate(float x, float y) noexcept
        {
            m_pos += {x,y};
            Generate();
        }

        const Vector2f& GetSize() const noexcept
        {
            return m_size;
        }

        const Vector2f& GetPosition() const noexcept
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
        virtual void Generate() noexcept = 0; // Generate std::vector<Vertex>
        Color m_color = WHITE;
        Vector2f m_pos = {0, 0};
        Vector2f m_size = {0, 0};
        Texture m_tex;
        float m_rotation = 0;
        std::vector<Vertex> m_vertexData;
        std::vector<ui32> m_indexData;
    };
}