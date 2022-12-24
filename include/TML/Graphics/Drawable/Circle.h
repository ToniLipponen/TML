#pragma once
#include <TML/Graphics/Drawable/Drawable.h>
#include <TML/System/Math.h>

namespace tml
{
    class TML_API Circle :  public Drawable
    {
    public:
        Circle();
        Circle(const Vector2f &pos, float radius);

        void SetRadius(float r);
        [[nodiscard]] float GetRadius() const noexcept;

    private:
        void OnDraw(class RenderTarget*, class Texture*) noexcept override;

    private:
        using Transformable::SetSize;
        using Transformable::GetSize;
    };
}