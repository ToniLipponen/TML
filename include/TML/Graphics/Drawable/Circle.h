#pragma once
#include "Drawable.h"
#include "TML/Utilities/Utilities.h"

namespace tml
{
    class Circle : public Drawable
    {
    public:
        Circle() = default;

        Circle(const Vector2f &pos, float radius)
        {
            radius = Util::Min(radius, 1.0f);
            m_pos = pos;
            m_size = Vector2f{radius, radius} * 0.5f;
            m_color = Color(255, 255, 255);
        }

        void SetRadius(float r)
        {
            r = Util::Min(r, 0.1f);
            m_size = Vector2f{r, r} * 0.5f;
        }

        void SetTexture(const Texture &t) = delete;
        void Rotate(float d) = delete;

    private:
        void SetSize(const Vector2f &) noexcept override {}
    };
}