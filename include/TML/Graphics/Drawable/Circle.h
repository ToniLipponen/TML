#pragma once
#include <TML/Graphics/Drawable/Drawable.h>
#include <TML/Utilities/Math.h>

namespace tml
{
    class Circle : public Drawable
    {
    public:
        Circle();
        Circle(const Vector2f &pos, float radius);

        void SetRadius(float r);
        float GetRadius() const noexcept;

        void SetTexture(const Texture &t)        = delete;
        void Rotate(float d)                     = delete;
        float GetRotation() const noexcept       = delete;
        void SetRotation(float d) noexcept       = delete;
        const Vector2f& GetSize() const noexcept = delete;

    private:
        void Generate() noexcept override;
        void SetSize(const Vector2f &) noexcept override {}
    };
}