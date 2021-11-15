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
            m_pos = {0, 0};
            m_size = {100, 100};
        }

        Rectangle(float x, float y, float w, float h)
        {
            m_pos = {x, y};
            m_size = {w, h};
        }

        Rectangle(const Vector2& position, const Vector2& size)
        {
            m_pos = position;
            m_size = size;
        }
    };
}