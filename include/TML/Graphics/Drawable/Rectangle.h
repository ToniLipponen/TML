#pragma once
#include <TML/Graphics/Drawable/Drawable.h>
#include <TML/Types.h>

namespace tml
{
    class Rectangle : public Drawable
    {
    public:
        Rectangle();
        Rectangle(float x, float y, float w, float h);
        Rectangle(const Vector2f& position, const Vector2f& size);
        void SetCornerRadius(float r) noexcept;
        float GetCornerRadius() const noexcept { return m_roundness; }

    protected:
        void OnDraw(class Renderer* renderer, class Texture*) noexcept override;
    protected:
        float m_roundness = 0;
    };
}