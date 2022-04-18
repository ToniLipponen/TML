#pragma once
#include <TML/Graphics/Drawable/Drawable.h>

namespace tml
{
    class Line : public Drawable
    {
    public:
        Line();
        Line(const Vector2f& a, const Vector2f& b, uint32_t thickness = 1, const Color& color = Color::White);

        void SetPointA(const Vector2f& point) noexcept;
        void SetPointB(const Vector2f& point) noexcept;
    private:
        void OnDraw(class Renderer*, Texture*) noexcept override;
    private:
        Vector2f a, b;
    };
}
