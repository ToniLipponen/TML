#pragma once
#include <TML/Graphics/Color.h>
#include <TML/Graphics/Drawable/Vertex.h>
#include <TML/Graphics/Transformable.h>
#include <vector>

namespace tml
{
    class TML_API Drawable : public Transformable
    {
    public:
        [[maybe_unused, nodiscard]] constexpr const Color& GetColor() const noexcept
        {
            return m_color;
        }

        virtual void SetColor(const Color &c) noexcept
        {
            /**
             * Checking if m_color is already the same as @param c.
             * Because this would otherwise invalidate the old vertex data.
             */
            if(m_color != c)
            {
                m_updated = true;
                m_color = c;
            }
        }

        friend class RenderTarget;

    protected:
        virtual void OnDraw(class RenderTarget*, class Texture*) noexcept = 0;

        Color m_color = Color::White;
        std::vector<Vertex> m_vertexData;
        std::vector<uint32_t> m_indexData;
    };
}