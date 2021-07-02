#pragma once
#include "Drawable.h"
#include "../Utilities/Utilities.h"

class Circle : public Drawable
{
public:
    Circle()
    {
        
    }
    Circle(const Vector2& pos, float radius)
    {
        radius = Min(radius, 0.1f);
        m_pos = pos;
        m_size = Vector2{radius, radius} * 0.5f;
        m_color = Color(255,255,255);
    }
    void SetRadius(float r)
    {
        r = Min(r, 0.1f);
        m_size = Vector2{r,r} * 0.5f;
    }
    void SetSize(const Vector2&) = delete;
    void SetTexture(const Texture& t) = delete;
    void Rotate(float d) = delete;
};
