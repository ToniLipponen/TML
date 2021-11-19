#pragma once
#include "Drawable.h"
#include "TML/Utilities/Types.h"

namespace tml
{
    class Rectangle : public Drawable
    {
    public:
        Rectangle()
        {
            m_pos = Vector2f{0.f, 0.f};
            m_size = Vector2f{100, 100};
        }

        Rectangle(float x, float y, float w, float h)
        {
            m_pos = Vector2f{x, y};
            m_size = Vector2f{w, h};
        }

        Rectangle(const Vector2f& position, const Vector2f& size)
        {
            m_pos = position;
            m_size = size;
        }
    };
}