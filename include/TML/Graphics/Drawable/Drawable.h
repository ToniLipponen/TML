#pragma once
#include <TML/Types.h>
#include <TML/Graphics/Drawable/Vertex.h>
#include <TML/Graphics/Transformable.h>
#include <vector>

namespace tml
{
    class Drawable : public Transformable
    {
    public:
        constexpr const Color& GetColor() const noexcept
        {
            return m_color;
        }

        void SetColor(const Color &c) noexcept
        {
            m_color = c;
            Generate();
        }

        Vector2f Move(const Vector2f& offset) noexcept override
        {
            Transformable::Move(offset);
            Generate();
            return m_pos;
        }

        Vector2f Move(float x, float y) noexcept override
        {
            Transformable::Move(x,y);
            Generate();
            return m_pos;
        }

        Vector2f SetPosition(const Vector2f& position) noexcept override
        {
            Transformable::SetPosition(position);
            Generate();
            return m_pos;
        }

        Vector2f SetPosition(float x, float y) noexcept override
        {
            Transformable::SetPosition(x,y);
            Generate();
            return m_pos;
        }

        Vector2f SetSize(const Vector2f& size) noexcept override
        {
            Transformable::SetSize(size.x, size.y);
            Generate();
            return m_size;
        }

        Vector2f SetSize(float x, float y) noexcept override
        {
            Transformable::SetSize(x,y);
            Generate();
            return m_size;
        }

        float Rotate(float degrees) noexcept override
        {
            Transformable::Rotate(degrees);
            Generate();
            return m_rotation;
        }

        float SetRotation(float degrees) noexcept override
        {
            Transformable::SetRotation(degrees);
            Generate();
            return m_rotation;
        }

        void SetOrigin(const Vector2f& origin) noexcept override
        {
            Transformable::SetOrigin(origin);
            Generate();
        }

        friend class Renderer;

    protected:
        virtual void Generate() noexcept = 0;
        Color m_color = Color::White;
        std::vector<Vertex> m_vertexData;
        std::vector<ui32> m_indexData;
    };
}