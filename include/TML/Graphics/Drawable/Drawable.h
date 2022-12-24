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
        virtual Color SetColor(const Color& color) noexcept;
        [[nodiscard]] Color GetColor() const noexcept;

        friend class RenderTarget;

    protected:
        virtual void OnDraw(class RenderTarget*, class Texture*) noexcept = 0;

        Color m_color = Color::White;
        std::vector<Vertex> m_vertexData;
        std::vector<uint32_t> m_indexData;
    };
}