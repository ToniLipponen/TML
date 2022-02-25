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
            m_updated = true;
        }

        friend class Renderer;

    protected:
        virtual void OnDraw(class Renderer* renderer, class Texture*) noexcept = 0;
        Color m_color = Color::White;
        std::vector<Vertex> m_vertexData;
        std::vector<ui32> m_indexData;
    };
}