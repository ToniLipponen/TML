#pragma once
#include "TML/Types.h"
#include "TML/Graphics/Core/Texture.h"
#include <TML/Graphics/Drawable/Vertex.h>
#include <vector>
#include <TML/Graphics/Transformable.h>

namespace tml
{
    class Drawable : public Transformable
    {
    public:
        constexpr const Color& GetColor() const noexcept
        {
            return m_color;
        }

        virtual void SetColor(const Color &c) noexcept
        {
            m_color = c;
            Generate();
        }

        virtual Vector2f Move(const Vector2f& offset) noexcept override
        {
            m_pos += offset;
            Generate();
            return m_pos;
        }

        virtual Vector2f Move(float x, float y) noexcept override
        {
            m_pos.x += x;
            m_pos.y += y;
            Generate();
            return m_pos;
        }

        virtual Vector2f SetPosition(const Vector2f& position) noexcept override
        {
            m_pos = position;
            Generate();
            return m_pos;
        }

        virtual Vector2f SetPosition(float x, float y) noexcept override
        {
            m_pos = {x, y};
            Generate();
            return m_pos;
        }

        virtual Vector2f Scale(const Vector2f& scale) noexcept override
        {
            m_size += scale;
            Generate();
            return m_size;
        }

        virtual Vector2f Scale(float x, float y) noexcept override
        {
            m_size.x += x;
            m_size.y += y;
            Generate();
            return m_size;
        }

        virtual Vector2f SetSize(const Vector2f& size) noexcept override
        {
            this->m_size = size;
            Generate();
            return m_size;
        }

        virtual Vector2f SetSize(float x, float y) noexcept override
        {
            m_size = {x, y};
            Generate();
            return m_size;
        }

        float Rotate(float degrees) noexcept override
        {
            m_rotation = std::fmod(m_rotation + degrees, 360.f);
            if (m_rotation < 0)
                m_rotation += 360.f;

            Generate();
            return m_rotation;
        }

        float SetRotation(float degrees) noexcept override
        {
            m_rotation = std::fmod(degrees, 360.f);
            if (m_rotation < 0)
                m_rotation += 360.f;

            Generate();
            return m_rotation;
        }

        friend class Renderer;

    protected:
        virtual void Generate() noexcept = 0; // Generate std::vector<Vertex>
        Color m_color = Color::White;
        Texture m_tex;
        std::vector<Vertex> m_vertexData;
        std::vector<ui32> m_indexData;
    };
}