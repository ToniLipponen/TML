#pragma once
#include "../Buffers.h"
#include "../Types.h"
#include "../Texture.h"

class Drawable
{
public:
    void SetColor(const Color& c)
    {
        m_color = c;
    }
    virtual void SetPosition(const Vector2& p)
    {
        m_pos = p;
    }
    virtual void SetSize(const Vector2& s)
    {
        m_size = s;
    }
    void SetTexture(const Texture& t)
    {
        m_tex = t;
    }
    void Rotate(float d)
    {
        m_rotation += d;
    }
    void Translate(const Vector2& offset)
    {
        m_pos += offset;
    }

    const Vector2& GetSize() const noexcept
    {
        return m_size;
    }

    const Vector2& GetPosition() const noexcept
    {
        return m_pos;
    }

    friend class Renderer;
protected:
    Color m_color;
    Vector2 m_pos;
    Vector2 m_size;
    Texture m_tex;
    float m_rotation = 0;
};