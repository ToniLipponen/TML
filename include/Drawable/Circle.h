#pragma once
#include "Drawable.h"



class Circle : public Drawable
{
public:
    Circle()
    {
        
    }
    Circle(const Vector2& pos, float radius)
    {
        m_pos = pos - Vector2{radius, radius};
        m_size = Vector2{radius, radius} * 2.f;
        m_color = Color(255,255,255);
    }
};
