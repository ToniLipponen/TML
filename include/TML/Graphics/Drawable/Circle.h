#pragma once
#include <TML/Graphics/Drawable/Drawable.h>
#include "TML/System/Math.h"

namespace tml
{
    class Circle : public Drawable
    {
    public:
        Circle();
        Circle(const Vector2f &pos, float radius);

        void SetRadius(float r);
        float GetRadius() const noexcept;

    private:
        void OnDraw(class Renderer* renderer, class Texture*) noexcept override;
        Vector2f SetSize(const Vector2f &)  noexcept{ return 0; }
        Vector2f SetSize(float,float)       noexcept{ return 0; }
    };
}