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
        void SetThickness(float thickness) noexcept;
        void SetRounded(bool rounded) noexcept;
        Vector2f GetCenter() noexcept override {return (a + b) / 2; }
        void SetSize(const Vector2f&) = delete;

    private:
        void OnDraw(class RenderTarget*, class Texture*) noexcept override;

    private:
        Vector2f a, b;
        bool m_rounded = false;
    };
}
