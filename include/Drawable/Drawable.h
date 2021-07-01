#pragma once
#include "../Buffers.h"
#include "../Types.h"
#include "../Texture.h"

struct TexRect
{
    Vector2 pos;
    Vector2 size;
};

class Drawable
{
public:
    void SetColor(const Color& c)
    {
        m_color = c;
    }
    void SetPosition(const Vector2& p)
    {
        m_pos = p;
    }
    void SetSize(const Vector2& s)
    {
        m_size = s;
    }
    virtual void SetTexture(const Texture& t)
    {
        m_tex = t;
    }
    void Rotate(float d)
    {
        m_rotation += d;
    }
    friend class Renderer;
protected:
    Color m_color;
    Vector2 m_pos;
    Vector2 m_size;
    TexRect m_rect;
    Texture m_tex;
    float m_rotation = 0;
};