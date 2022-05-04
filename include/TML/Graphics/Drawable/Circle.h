#pragma once
#include <TML/Graphics/Drawable/Drawable.h>
#include <TML/System/Math.h>

namespace tml
{
    class TML_API Circle : public Drawable
    {
    public:
        Circle();
        Circle(const Vector2f &pos, float radius);

        void SetRadius(float r);
        float GetRadius() const noexcept;

        Vector2f SetSize(const Vector2f &)  noexcept = delete;
        Vector2f SetSize(float,float)       noexcept = delete;

    private:
        void OnDraw(class Renderer* renderer, class Texture*) noexcept override;
    };
}