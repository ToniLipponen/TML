#pragma once
#include <TML/Vector2.h>

namespace tml::Interface
{
    class Object
    {
    public:
        Object() : m_pos(0), m_size(0){}
        void SetPosition(const Vector2& position)
        {
            m_pos = position;
        }
        void SetSize(const Vector2& size)
        {
            m_size = size;
        }
        inline Vector2 GetPosition() const
        {
            return m_pos;
        }
        inline Vector2 GetSize() const
        {
            return m_size;
        }
    protected:
        Vector2 m_pos;
        Vector2 m_size;
    };
}