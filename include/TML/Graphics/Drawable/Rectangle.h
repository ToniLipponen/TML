#pragma once
#include <TML/Graphics/Drawable/Drawable.h>
#include "TML/Types.h"

namespace tml
{
    class Rectangle : public Drawable
    {
    public:
        Rectangle();
        Rectangle(float x, float y, float w, float h);
        Rectangle(const Vector2f& position, const Vector2f& size);

    protected:
        void Generate() noexcept override;
    };
}