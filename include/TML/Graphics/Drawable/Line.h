#pragma once

#include <TML/Graphics/Drawable/Drawable.h>

namespace tml
{
    class Line : public Drawable
    {
    public:
        Line();
        Line(const Vector2f& a, const Vector2f& b, ui32 thickness = 1, const Color& color = Color::White);

        void SetPointA(const Vector2f& point) noexcept;
        void SetPointB(const Vector2f& point) noexcept;
    private:
        void Generate() noexcept override;
    private:
        Vector2f a, b;
    };
}
