#pragma once
#include <TML/Graphics/Drawable/Drawable.h>
#include <TML/Export.h>

namespace tml
{
    class TML_API Line : public Drawable
    {
    public:
        Line();
        Line(const Vector2f& a, const Vector2f& b, float thickness = 1, const Color& color = Color::White);

        void SetPointA(const Vector2f& point) noexcept;
        void SetPointB(const Vector2f& point) noexcept;
    private:
        void OnDraw(class Renderer*, Texture*) noexcept override;
    private:
        Vector2f a, b;
    };
}
